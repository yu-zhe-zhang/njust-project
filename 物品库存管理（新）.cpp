//物品库存管理系统
//张宇哲 9211630R0129
#include<iostream>
#include<string>
#include<fstream>
#include<time.h>
#include<stdio.h>
#define MAXNUM 10 //一个品种中最多存放10个品牌
#define CpctMax 100000   //定义仓库容量为十万个单位
using namespace std;
class Brand //品牌类
{
private:
    char brand_name[10]; //品牌名
    int brand_code; //品牌代号
    int quantity; //数量
    float brnd_price; //价格
public:
    friend class CItem;//定义品种类的友元
    friend class Mangercangku;//定义仓库管理类为友元
};
class CItem //品种类
{
private:
    char Item_name[10]; //品种名称
    int Item_code; //品种代号
    char colour[10]; //颜色
    int quantity; //数量
    Brand brnd[MAXNUM]; //最多MAXNUM个品牌
    int nBrnd; //品牌的数量
    int ocpycpct;//记录品种的单位容量，即该品种中一件商品所占仓库空间的大小
public:
    friend class Mangercangku;//定义仓库管理类为友元
    friend struct Client;//定义客户类为友元
};
class Mangercangku //仓库管理类
{
    CItem c1,c3;//定义两个品种类的对象
    CItem c_Array[100];//存放100个品种
    int c_num; //品种数量
    int capacity;//记录仓库中已有物品所占的容量
public:
    Mangercangku()
    {
        ifstream ifs;
        ifs.open("TIS.txt",ios::in);
        if(!ifs.is_open())//判断文件是否存在
        {
            cout<<"文件不存在！\n";
            this->c_num=0;
            this->capacity=0;
            ifs.close();
            return;
        }
        if(ifs.eof())//判断文件是否为空
        {
            cout<<"文件为空！\n";
            this->c_num=0;
            this->capacity=0;
            ifs.close();
            return;
        }
        else
        {
            int num=this->get_Cnum();//初始化品种数
            this->c_num=num;//更新品种数
            cout<<"仓库中的品种数为： "<<this->c_num<<endl;
            this->IntiCItem();//初始化品种数据
            this->capacity=0;
            for(int j=0;j<num;j++)//初始化仓库容量
            {
                this->capacity+=(this->c_Array[j].quantity*this->c_Array[j].ocpycpct);
            }
            cout<<"仓库已占容量/仓库容量： "<<this->capacity<<'/'<<CpctMax<<"\n";
        }
    }

    void ShowMenu() //显示菜单
    {
        cout<<"***************************"<<endl;
        cout<<"欢迎使用物品库存管理系统！*"<<endl;
        cout<<"***** 1、添加商品品种 *****"<<endl;
        cout<<"***** 2、显示商品品种 *****"<<endl;
        cout<<"***** 3、查询商品     *****"<<endl;
        cout<<"***** 4、删除商品     *****"<<endl;
        cout<<"***** 5、商品出库     *****"<<endl;
        cout<<"***** 6、商品进库     *****"<<endl;
        cout<<"***** 7、查询出货清单 *****"<<endl;
        cout<<"***** 8、查询进货清单 *****"<<endl;
        cout<<"***** 0、退出系统     *****"<<endl;
        cout<<"***************************"<<endl;
    }

    void AddCItem() //添加功能
    {
        char name[10];//定义品种名称
        int code;//定义品种代码，
        cout<<"请输入品种名称： ";
        cin>>name;
        cout<<"请输入品种代码： ";
        cin>>code;//接收用户输入的数据
        if(this->RelyIcode(code)!=-1 )//如果添加的是库中已经存在的品种，那么在原先的数据上继续添加
        {
            cout<<"库中已有此品种！\n";
            FLAG20:
            cout<<"请输入添加的品牌数量：  ";
            int nnn,xxx,yyy;//nnn是接收新添加的品牌数，xxx是接收找到的品种在数组中的下标，yyy是用来暂时存放品种中的品牌数的
            xxx=this->RelyIcode(code);//找到品种在数组中的下标
            cin>>nnn;//新添加的品牌数
            yyy=this->c_Array[xxx].nBrnd+nnn;//暂时存放品种中的品牌数
            if(yyy>10)
            {
                cout<<"添加的品牌数目过多！\n";
                goto FLAG20;//返回重新输入品牌数
            }
            for(int i=0;i<nnn;i++)
            {
                char nnnum[10];//接受品牌名
                int bbcode;//接受品牌代码
                cout<<"请输入第"<<i+1<<"个品牌的信息：\n名称： ";
                cin>>nnnum;
                cout<<"代码： ";
                cin>>bbcode;
                //添加的是原来存在的品牌
                if(this->RelyIBcode(bbcode,xxx)!=-1)
                {
                    int xia;//接收品牌数组的下标
                    xia=this->RelyIBcode(bbcode,xxx);
                    cout<<"库中已有此品牌\n请输入入库数量： ";
                    int nnuumm;//接收入库数量
                    cin>>nnuumm;
                    this->c_Array[xxx].brnd[xia].quantity+=nnuumm;//计算入库后的品牌的库存数量
                    this->c_Array[xxx].quantity+=nnuumm;//计算品种的商品数
                    this->WriteInold(xxx,nnuumm,xia);//写进货清单
                }
                else//添加新的品牌
                {
                    strcpy(c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].brand_name,nnnum);//将接收的品牌名复制到品种里的品牌上
                    this->c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].brand_code=bbcode;//将接收的品牌代码复制到品种里的品牌上
                    cout<<"数量： ";
                    cin>>this->c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].quantity;//接收品牌数量
                    cout<<"价格： ";
                    cin>>this->c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].brnd_price;//接收品牌价格
                    this->c_Array[xxx].quantity+=this->c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].quantity;//更新品牌中的商品数
                    this->c_Array[xxx].nBrnd++;//更新品牌数
                    this->WriteInold(xxx,this->c_Array[xxx].brnd[this->c_Array[xxx].nBrnd].quantity,this->c_Array[xxx].nBrnd);//写进货清单
                }
            }
            this->save();//将数据保存至文件中
            cout<<"添加成功！\n";
            this->jixu();//按回车继续的操作
            return;
        }
        //添加全新品种
        this->c_num++;//更新品种数
        strcpy(this->c_Array[this->c_num-1].Item_name,name);//将接收的品牌名复制到品种里的品牌上
        this->c_Array[this->c_num-1].Item_code=code;//将接收的品牌代码复制到品种里的品牌上
        cout<<"请输入品种颜色： ";
        cin>>this->c_Array[this->c_num-1].colour;//接收颜色
        cout<<"请输入该品种一个商品所占仓库的容积： ";
        cin>>this->c_Array[this->c_num-1].ocpycpct;//接收一个商品所占仓库容量的大小
        FLAG14:
        cout<<"请输入品种包含的品牌数量： ";
        cin>>this->c_Array[this->c_num-1].nBrnd;//接收品牌数
        if(this->c_Array[this->c_num-1].nBrnd>10)//判断所添加的品牌数是否超出上限
        {
            cout<<"添加的品牌数过多！请重新输入数据！\n";
            goto FLAG14;//跳转到输入品牌数的位置
        }
        for(int i=0;i<this->c_Array[this->c_num-1].nBrnd;i++)
        {
            int g=1;//与库存有关的标志
            cout<<"请输入第"<<i+1<<"个品牌的信息：\n名称： ";
            cin>>this->c_Array[this->c_num-1].brnd[i].brand_name;//接收品牌名称
            cout<<"代号： ";
            cin>>this->c_Array[this->c_num-1].brnd[i].brand_code;//接收品牌代码
            FLAG40:
            cout<<"数量： ";
            cin>>this->c_Array[this->c_num-1].brnd[i].quantity;//接收品牌的商品数
            this->c_Array[this->c_num-1].quantity+=this->c_Array[this->c_num-1].brnd[i].quantity;//更新品种中的商品数
            //当发现放不下且此时并不是用户添加的最后一个品牌信息时
            if(this->c_Array[this->c_num-1].quantity*this->c_Array[this->c_num-1].ocpycpct+this->capacity>CpctMax&&i<this->c_Array[this->c_num-1].nBrnd-1)
            {
                cout<<"仓库空间不足！最多能添加"<<(CpctMax-this->capacity)/(this->c_Array[this->c_num-1].ocpycpct)<<"件商品\n";
                int g=0;//仓库已满的标志
                this->c_Array[this->c_num-1].nBrnd=i+1;//更改品牌数量
                goto FLAG40;//跳转到输入商品数量的位置
            }
            //发现放不下，并且此时正在添加最后一个品牌的信息时
            else if(this->c_Array[this->c_num-1].quantity*this->c_Array[this->c_num-1].ocpycpct+this->capacity>CpctMax&&i==this->c_Array[this->c_num-1].nBrnd-1)
            {
                cout<<"仓库空间不足！最多能添加"<<(CpctMax-this->capacity)/(this->c_Array[this->c_num-1].ocpycpct)<<"件商品\n";
                
                int g=0;//仓库已满的标志
                this->c_Array[this->c_num-1].nBrnd=i+1;//更改品牌数量
                goto FLAG40;
            }
            //发现刚好放下时
            else if(this->c_Array[this->c_num-1].quantity*this->c_Array[this->c_num-1].ocpycpct+this->capacity==CpctMax&&i==this->c_Array[this->c_num-1].nBrnd-1)
            {
                cout<<"仓库已填满！无法再次进货了！\n";
                int g=0;//仓库已满的标志
                this->c_Array[this->c_num-1].nBrnd=i+1;//更改品牌数量
            }
            cout<<"价格： ";
            cin>>this->c_Array[this->c_num-1].brnd[i].brnd_price;//接收品牌价格
            this->WriteInold(this->c_num-1,this->c_Array[this->c_num-1].brnd[i].quantity,i);//写进货清单
            if(g==0)//判断仓库是否已满
            {
                break;
            }
        }
        this->save();//实行保存功能
        cout<<"添加成功！\n";
        this->jixu();//按回车继续的操作
    }

    int get_Cnum() //获取文件中的品种数
    {
        ifstream ifs;
        //打开文件
        ifs.open("/Users/zhanghong/C++课程设计/TIS.txt",ios::in);
        int num=0;
        CItem p1;//接收读取的数据
        ifs.read((char *)&p1,sizeof(CItem));
        while(!ifs.eof())
        {
            this->c_Array[num]=p1;//将接收的数据存放到数组中
            ifs.read((char *)&p1,sizeof(CItem));
            num++;//更新数组下标
        }
        ifs.close();//关闭文件
        return num;
    }
    
    void IntiCItem() //初始化数据
    {
        ifstream ifs;
        //打开文件
        ifs.open("/Users/zhanghong/C++课程设计/TIS.txt",ios::in);
        if(!ifs.is_open())
        {
            cout<<"文件TIS不存在或文件为空！\n";
        }
        else
        {
            int num=0;
            CItem p1;//接收读取的数据
            ifs.read((char *)&p1,sizeof(CItem));
            while(!ifs.eof())
            {
                this->c_Array[num]=p1;//将接收的数据存放到数组中
                ifs.read((char *)&p1,sizeof(CItem));
                num++;//更新数组下标
            }
        }
        ifs.close();//关闭文件
    }

    void ShowCItem() //显示品种信息
    {
        if(this->c_num==0)//如果品种数为0，则证明仓库为空，返回主页面
        {
            cout<<"仓库为空！\n";
            this->jixu();
            return;
        }
        for(int i=0;i<this->c_num;i++)//若不为空，则输出数据
        {
            cout<<"品种名称：  "<<this->c_Array[i].Item_name
                <<"  品种代号：  "<<this->c_Array[i].Item_code
                <<"  品种颜色：  "<<this->c_Array[i].colour
                <<"\n   该品种所占的单位容积：  "<<this->c_Array[i].ocpycpct
                <<"  品种数量：  "<<this->c_Array[i].quantity;
            for(int j=0;j<this->c_Array[i].nBrnd;j++)
            {
                cout<<"\n\t第"<<j+1<<"个品牌的信息：\n\t名称：  "<<c_Array[i].brnd[j].brand_name
                    <<"  代号：  "<<this->c_Array[i].brnd[j].brand_code
                    <<"  价格：  "<<this->c_Array[i].brnd[j].brnd_price
                    <<"  数量：  "<<this->c_Array[i].brnd[j].quantity;
            }
            cout<<endl;
        }
        cout<<endl;
        this->jixu();
    }

    void jixu()//按Enter键继续
    {
        cout<<"请按Enter键继续......\n";
        getchar();
    }

    void save() //保存功能
    {
        for(int j=0;j<this->c_num;j++)//按品种代码，对品种数组进行冒泡排序
        {
            for(int k=0;k<this->c_num-j-1;k++)
            {
                if(this->c_Array[k].Item_code>this->c_Array[k+1].Item_code)
                {
                    CItem temp=this->c_Array[k+1];
                    this->c_Array[k+1]=this->c_Array[k];
                    this->c_Array[k]=temp;
                }
            }
        }
        int i=0;
        ofstream ofs;
        //打开文件，如果文件存在，先删除，再创建一个新文件，存放数据
        ofs.open("/Users/zhanghong/C++课程设计/TIS.txt",ios::out|ios::trunc);
        CItem cc;
        while(i<this->c_num)
        {
            cc=this->c_Array[i];
            ofs.write((char *)&cc,sizeof(CItem));
            i++;
        }
        ofs.close();//关闭文件
    }

    int RelyIname(string name)//按照品种名字查找，并返回在数组中的下标,如果没有找到，返回-1
    {
        int g=-1;
        for(int i=0;i<this->c_num;i++)
        {
            if(this->c_Array[i].Item_name==name)//如果发现品种数组中有品种的名称与传入的名称一致时，返回品种数组下标
            {
                return i;
            }
            else//否则返回-1
            {
                g=-1;
            }
        }
        return g;
    }

    int RelyIcode(int code)//按照品种代号查找，并返回下标,如果没有找到，返回-1
    {
        int g;
        if(this->c_num==0)//如果仓库中没有品种，则返回-1
        {
            return -1;
        }
        for(int i=0;i<this->c_num;i++)
        {
            if(this->c_Array[i].Item_code==code)//如果发现品种数组中有品种的代码与传入的代码一致时，返回品种数组下标
            {
                return i;
            }
            else//否则返回-1
            {
                g=-1;
            }
        }
        return g;
    }

    int RelyIBname(string name,int i)//按品牌名查找，并返回下标，如无，返回-1
    {
        int k=-1;
        for(int j=0;j<this->c_Array[i].nBrnd;j++)
        {
            if(this->c_Array[i].brnd[j].brand_name==name)//如果发现品种数组中有品种的名称与传入的名称一致时，返回品种数组下标
            {
                return j;
            }
            else//否则返回-1
            {
                k=-1;
            }
        }
        return k;
    }

    int RelyIBcode(int code,int i)//按品牌代号查找，并返回下标，如无，返回-1
    {
        for(int j=0;j<this->c_Array[i].nBrnd;j++)
        {
            if(this->c_Array[i].brnd[j].brand_code==code)//如果发现品种数组中有品种的代码与传入的代码一致时，返回品种数组下标
            {
                return j;
            }
        }
        return -1;
    }
    
    void PrintInfo(int i)//打印信息,i是品种数组的下标
    {
        cout<<"品种名称：  "<<this->c_Array[i].Item_name
                <<"  品种代号：  "<<this->c_Array[i].Item_code
                <<"  品种颜色：  "<<this->c_Array[i].colour
                <<"  品种数量：  "<<this->c_Array[i].quantity;
        for(int j=0;j<c_Array[i].nBrnd;j++)
        {
            cout<<"\n\t第"<<j+1<<"个品牌的信息：\n\t名称：  "<<c_Array[i].brnd[j].brand_name
                <<"  代号：  "<<this->c_Array[i].brnd[j].brand_code
                <<"  价格：  "<<this->c_Array[i].brnd[j].brnd_price
                <<"  数量：  "<<this->c_Array[i].brnd[j].quantity<<endl;
        }
    }
    
    void SeekItem()//查找功能
    {
        FLAG1:
        cout<<"\t1、按商品品种代号查找商品\n\t2、按商品品种名称查询商品\n\t3、查找某品牌在库中的全部信息\n\t0、退出\n";
        int s;
        cin>>s;
        switch(s)
        {
        case 1://按品种代号找
            {
                cout<<"请输入品种代号：";
                int code;//接收品种代号
                cin>>code;
                int i=RelyIcode(code);//查找品种，并返回品种数组的下标
                if(i==-1)
                {
                    cout<<"查无此物！\n";
                }
                else
                {
                    PrintInfo(i);//打印品种信息
                }
                break;
            }
        case 2://按品种名称找
            {
                cout<<"请输入品种名称：";
                char name[10];//接收品种名称
                cin>>name;
                int n=RelyIname(name);
                if(n==-1)
                {
                    cout<<"查无此物！\n";
                }
                else
                {
                    PrintInfo(n);//打印信息
                }
                break;
            }
        case 3://查找某一品牌在库中的全部信息
            {
                int pan=1;
                cout<<"请输入所要查找的品牌的名称：  ";
                string Bnnammee;
                cin>>Bnnammee;//接收品牌名
                for(int i=0;i<this->c_num;i++)
                {
                    for(int k=0;k<this->c_Array[i].nBrnd;k++)
                    {
                        if(this->c_Array[i].brnd[k].brand_name==Bnnammee)
                        {
                            pan=0;
                            this->PrintBallifo(i,k);//打印品牌信息
                        }
                    }
                }
                if(pan)
                {
                    cout<<"没有找到此品牌！\n";
                    this->jixu();
                    return;
                }
                break;
            }
        case 0:
            return;
        default:
            cout<<"请输入正确的选项！\n";
            goto FLAG1;//返回选项页面
        }
        this->jixu();//按enter键继续操作
    }

    void PrintBallifo(int I,int B)//I用来接收品种数组下标，B用来接收品牌数组下标
    {
        cout<<"\t所属品种名称： "<<this->c_Array[I].Item_name
            <<" 所属品种代码： "<<this->c_Array[I].Item_code
            <<" 颜色： "<<this->c_Array[I].colour
            <<"\n\t品牌名： "<<this->c_Array[I].brnd[B].brand_name
            <<" 在此品种中的代码： "<<this->c_Array[I].brnd[B].brand_code
            <<" 在此品种中的数量： "<<this->c_Array[I].brnd[B].quantity
            <<" 在此品种中的价格： "<<this->c_Array[I].brnd[B].brnd_price<<endl;
    }

    void DeleteItem()//删除功能
    {
        FLAG2:
        cout<<"\t1、删除商品品种\n\t2、删除商品品牌\n\t0、退出\n";
        int s;
        cin>>s;
        switch(s)
        {
        case 1://删除品种
            {
                FLAG3:
                cout<<"\t1、按商品品种代码删除商品\n\t2、按商品品种名称删除商品\n\t0、退出\n";
                int a;
                cin>>a;
                switch(a)
                {
                case 1://按代码删品种
                    {
                        cout<<"请输入所要删除品种的代码：   ";
                        int b,c;
                        cin>>b;
                        c=RelyIcode(b);//查找品种数组下标
                        for(int d=c;d<this->c_num;d++)
                        {
                            this->c_Array[d]=this->c_Array[d+1];
                        }
                        this->c_num--;
                        cout<<"删除成功！\n";
                        break;
                    }
                case 2://按名称删品种
                    {
                        cout<<"请输入所要删除品种的名称：   ";
                        string e;
                        cin>>e;
                        int f=RelyIname(e);
                        for(int g=f;g<this->c_num;g++)
                        {
                            this->c_Array[g]=this->c_Array[g+1];
                        }
                        this->c_num--;
                        cout<<"删除成功！\n";
                        break;
                    }
                case 0://退出
                    return;
                default://选项有错误
                    cout<<"请输入正确的选项！\n";
                    goto FLAG3;
                }
                break;
            }
        case 2://删除品牌
            {
                FLAG7:
                cout<<"\t1、按商品品种代码删除商品\n\t2、按商品品种名称删除商品\n\t0、退出\n";
                int a;
                cin>>a;
                switch(a)
                {
                case 1://按代码查品种
                    {
                        cout<<"请输入所要删除品种的代码：   ";
                        int b,c,d;//b接收品种代码，c接收品种在数组中的下标，d接收选项
                        cin>>b;
                        c=RelyIcode(b);//找出品种在数组中的下标
                        if(c==-1)
                        {
                            cout<<"查无此物！\n";
                            return;
                        }
                        FLAG4:
                        cout<<"\t1、按品牌名删除品牌\n\t2、按品牌代码删除品牌\n\t0、退出\n";
                        cin>>d;
                        switch(d)
                        {
                        case 1://按名称删品牌
                            {
                                cout<<"请输入要删除品牌的名称：   ";
                                string bname;//接收品牌名
                                cin>>bname;
                                int e;//接收品牌在品牌数组中的下标
                                e=RelyIBname(bname,c);//找到品牌在品牌数组中的下标
                                if(e==-1)
                                {
                                    cout<<"没有找到该品牌！\n";
                                    return;
                                }
                                for(int f=e;f<this->c_Array[c].nBrnd;f++)//执行删除操作
                                {
                                    this->c_Array[c].brnd[f]=this->c_Array[c].brnd[f+1];
                                }
                                this->c_Array[c].nBrnd--;
                                cout<<"删除成功！\n";
                                break;
                            }
                        case 2://按代码删品牌
                            {
                                cout<<"请输入要删除品牌的代码：   ";
                                int g;//接收品牌代码
                                cin>>g;
                                int h;//接收品牌在品牌数组中的下标
                                h=RelyIBcode(g,c);//找到品牌在品牌数组中的下标
                                if(h==-1)
                                {
                                    cout<<"没有找到该品牌！\n";
                                    return;
                                }
                                for(int i=h;i<this->c_Array[c].nBrnd;i++)//执行删除操作
                                {
                                    this->c_Array[c].brnd[i]=this->c_Array[c].brnd[i+1];
                                }
                                this->c_Array[c].nBrnd--;
                                cout<<"删除成功！\n";
                                break;
                            }
                        case 0://退出
                            return;
                        default:
                            cout<<"请输入正确的选项！\n";
                            goto FLAG4;
                        }
                        break;
                    }
                case 2://按名称查品种
                    {
                        cout<<"请输入所要删除品种的名称：   ";
                        char j[10];//接收品种名称
                        cin>>j;
                        int k=RelyIname(j);//找到品种在数组中的下标
                        if(k==-1)
                        {
                            cout<<"查无此物！\n";
                            return;
                        }
                        FLAG5:
                        cout<<"\t1、按品牌名删除品牌\n\t2、按品牌代码删除品牌\n\t0、退出\n";
                        int l;//接收选项
                        cin>>l;
                        switch(l)
                        {
                        case 1://按名称删
                            {
                                cout<<"请输入要删除品牌的名称：   ";
                                char bbname[10];
                                cin>>bbname;
                                int m=RelyIBname(bbname,k);//找到品牌在品牌数组中的下标
                                if(m==-1)
                                {
                                    cout<<"没有找到该品牌！\n";
                                    return;
                                }
                                for(int n=m;n<this->c_Array[k].nBrnd;n++)//删除操作
                                {
                                    this->c_Array[k].brnd[n]=this->c_Array[k].brnd[n+1];
                                }
                                this->c_Array[k].nBrnd--;
                                cout<<"删除成功！\n";
                                break;
                            }
                        case 2://按代码删
                            {
                                cout<<"请输入要删除品牌的代码：   ";
                                int bbcode;//接收品牌代码
                                cin>>bbcode;
                                int m=RelyIBcode(bbcode,k);//找到品牌在品牌数组中的下标
                                if(m==-1)
                                {
                                    cout<<"没有找到该品牌！\n";
                                    return;
                                }
                                for(int n=m;n<this->c_Array[k].nBrnd;n++)//删除操作
                                {
                                    this->c_Array[k].brnd[n]=this->c_Array[k].brnd[n+1];
                                }
                                this->c_Array[k].nBrnd--;
                                cout<<"删除成功！\n";
                                break;
                            }
                        case 0:
                            return;
                        default:
                            cout<<"请 输入正确的选项！\n";
                            goto FLAG5;
                        }
                        break;
                    }
                case 0://退出
                    return;
                default://选项有错误
                    cout<<"请输入正确的选项！\n";
                    goto FLAG7;
                }
                break;
            }
        case 0://退出
            break;
        default:
            cout<<"请输入正确的选项！\n";
            goto FLAG2;
        }
        this->save();
        this->jixu();
    }

    void ExitSystem() //退出功能
    {
        cout<<"欢迎下次使用^_^\n";
        exit(0);
    }

    int WriteExitfile(int a,int b,int num,float price)//写出货清单具体操作，a是品种数组下标，b是品牌数组下标,num是出货数量
    {
        time_t timer;//定义时间类对象
        timer=time(NULL); //取得当前时间
        this->c_Array[a].brnd[b].quantity-=num;
        this->c_Array[a].quantity-=num;
        price=price+this->c_Array[a].brnd[b].brnd_price*num;//计算价格
        cout<<"商品出库成功！\n";
        ofstream ofs;
        ofs.open("/Users/zhanghong/C++课程设计/Exitfile.txt",ios::out|ios::app);
        if(!ofs.is_open())
        {
            cout<<"文件不存在或文件打开失败！\n";
            return 0;
        }
        ofs<<ctime(&timer)<<"\n品种名称： "<<this->c_Array[a].Item_name
            <<" 品种代码： "<<this->c_Array[a].Item_code<<" 品种颜色： "<<this->c_Array[a].colour
            <<"\n\t出货品牌： "<<this->c_Array[a].brnd[b].brand_name
            <<" 出货品牌代码： "<<this->c_Array[a].brnd[b].brand_code
            <<" 出货数量： "<<num<<endl;
        ofs.close();
        return price;
    }
    
    void WriteExitifobyBname(int a) //按品牌名在文件中写出货清单,传入品种数组下标即可
    {
        float price=0;//记录提取货物总价格
        FLAG9:
        cout<<"请输入品牌名：  ";
        char Bname[10];
        cin>>Bname;
        int b=RelyIBname(Bname,a);//找品牌在数组中的下标
        if(b==-1)
        {
            cout<<"查无此物！\n";
            return;
        }
        cout<<"请输入出货数量：  ";
        int num;
        cin>>num;
        if(num>this->c_Array[a].brnd[b].quantity)
        {
            cout<<"商品库存不够！\n";
        }
        else
        {
            //传品种数组下标a，品牌数组下标b，出库数量num
            price=this->WriteExitfile(a,b,num,price);
            if(price<=0) return;
        }
        cout<<"是否继续查询商品出库？\n1、是\t2、否\n";
        int Isctn;
        cin>>Isctn;
        if(Isctn==1) goto FLAG9;
        ofstream ofs;
        ofs.open("/Users/zhanghong/C++课程设计/Exitfile.txt",ios::out|ios::app);
        ofs<<"提货总价格为： "<<price<<"元\n";
        ofs.close();
        this->jixu();
    }

    void WriteExitifobycode(int a) //按品牌代号在文件中写出货清单
    {
        float price=0;//记录提取货物总价格
        FLAG10:
        cout<<"请输入品牌代号：  ";
        int Bcode;
        cin>>Bcode;
        int b=RelyIBcode(Bcode,a);//找品牌在数组中的下标
        if(b==-1)
        {
            cout<<"查无此物！\n";
            return;
        }
        cout<<"请输入出货数量：  ";
        int num;
        cin>>num;
        if(num>this->c_Array[a].brnd[b].quantity)
        {
            cout<<"商品库存不够！\n";
        }
        else
        {
            price=this->WriteExitfile(a,b,num,price);
            if(price<=0) return;
        }
        cout<<"是否继续查询商品出库？\n1、是\t2、否\n";
        int Isctn;
        cin>>Isctn;
        if(Isctn==1) goto FLAG10;
        ofstream ofs;
        ofs.open("/Users/zhanghong/C++课程设计/Exitfile.txt",ios::out|ios::app);
        ofs<<"提货总价格为： "<<price<<"元\n";
        ofs.close();
        this->jixu();
    }

    void ItemExit() //商品出库功能
    {
        FLAG6:
        cout<<"\t1、按品种名称查询商品出库\n\t2、按品种代码查询商品出库\n\t0、退出\n";
        int select;
        cin>>select;
        switch(select)
        {
        case 1://按品种名称
            {
                cout<<"请输入品种名称：  ";
                string Iname;
                cin>>Iname;
                int a=RelyIname(Iname);//接收数组下标
                if(a==-1)
                {
                    cout<<"查无此物！\n";
                    this->jixu();
                    return;
                }
                this->PrintInfo(a);//打印所要出库品种的信息
                FLAG8:
                cout<<"\t1、按品牌名查询商品出库\n\t2、按品牌代号查询商品出库\n\t0、退出\n";
                int select;
                cin>>select;
                switch(select)
                {
                case 1://按品牌名找
                    {
                        this->WriteExitifobyBname(a);
                    }
                case 2://按品牌号找
                    {
                        this->WriteExitifobycode(a);
                    }
                case 0:
                    return;
                default:
                    cout<<"请输入正确的选项！\n";
                    goto FLAG8;
                }
                break;
            }
        case 2://按品种代码
            {
                float price=0;//记录提取货物总价格
                cout<<"请输入品种代码：  ";
                int Icode;
                cin>>Icode;
                int a=RelyIcode(Icode);//接收数组下标
                if(a==-1)
                {
                    cout<<"查无此物！\n";
                    this->jixu();
                    return;
                }
                this->PrintInfo(a);//打印所要出库品种的信息
                FLAG11:
                cout<<"\t1、按品牌名查询商品出库\n\t2、按品牌代号查询商品出库\n\t0、退出\n";
                int select;
                cin>>select;
                switch(select)
                {
                case 1://按品牌名找
                    {
                        this->WriteExitifobyBname(a);
                    }
                case 2://按品牌号找
                    {
                        this->WriteExitifobycode(a);
                    }
                case 0:
                    return;
                default:
                    cout<<"请输入正确的选项！\n";
                    goto FLAG11;
                }
                break;
            }
        case 0:
            return;
        default:
            cout<<"请输入正确的选项！\n";
            goto FLAG6;
        }
        cout<<"商品出库成功！\n";
        this->jixu();
    }
    
    void WriteInold(int a,int n,int b) //进的货为库中存在的时，写进货清单 a是品种数组的下标，n是进货数量，b是品牌数组下标
    {
        time_t timer;//定义时间类对象
        timer=time(NULL);
        ofstream ofs;
        ofs.open("Infile.txt",ios::out|ios::app);
        if(!ofs.is_open())
        {
            cout<<"文件不存在或文件打开失败！\n";
            return;
        }
        ofs<<ctime(&timer)<<"\t"<<"品种代码： "<<this->c_Array[a].Item_code
            <<"\t品种名称： "<<this->c_Array[a].Item_name<<"\t品种颜色： "<<this->c_Array[a].colour
            <<"\t进货的品牌： "<<this->c_Array[a].brnd[b].brand_name<<"\t品牌代码： "<<this->c_Array[a].brnd[b].brand_code
            <<"\t进货数量为：  "<<n<<endl;
        ofs.close();
    }

    void ItemIn() //商品入库功能
    {
        cout<<"请输入品种代号： ";
        int Icodee;
        cin>>Icodee;
        int aa=this->RelyIcode(Icodee);//品种数组下标
        if(aa!=-1)//库中已经有此品种
        {
            cout<<"库中已有此品种\n";
            cout<<"请输入入库品牌代号：   ";
            int Bcodee;
            cin>>Bcodee;
            int bb=this->RelyIBcode(Bcodee,aa);//品牌数组下标
            if(bb!=-1)//库中已经有此品牌
            {
                cout<<"库中已经有此品牌\n";
                FLAGG:
                cout<<"请输入进货数量： ";
                int numm;
                cin>>numm;
                //防止超出仓库容量
                if(numm*this->c_Array[aa].ocpycpct>CpctMax-this->capacity)
                {
                    cout<<"进货数量过多！仓库容量不足！\n最多还能入库的数量为： "<<(CpctMax-this->capacity)/this->c_Array[aa].ocpycpct<<endl;
                    cout<<"请输入您的选择\n1、重新输入进货数量\n2、退出入库程序\n";
                    int sss;
                    cin>>sss;
                    if(sss==1) goto FLAGG;
                    else return;
                }
                this->c_Array[aa].quantity+=numm;
                this->c_Array[aa].brnd[bb].quantity+=numm;
                this->capacity+=numm*this->c_Array[aa].ocpycpct;//更新货物所占容量
                this->WriteInold(aa,numm,bb);//写进货清单
            }
            else//添加新品牌
            {
                if(this->c_Array[aa].nBrnd+1>10)//如果添加新品牌前，品牌数已经达到最大，那么显示放不下
                {
                    cout<<"仓库已满！\n";
                    return;
                }
                char bbbma[10];//接收品牌名
                int hahaha;//接收入库数量
                cout<<"请输入品牌名称： ";
                cin>>bbbma;
                FLAGGG:
                cout<<"请输入入库数量： ";
                cin>>hahaha;
                if(hahaha*this->c_Array[aa].ocpycpct>CpctMax-this->capacity)
                {
                    cout<<"进货数量过多！仓库容量不足！\n最多还能入库的数量为： "<<(CpctMax-this->capacity)/this->c_Array[aa].ocpycpct<<endl;
                    cout<<"请输入您的选择\n1、重新输入进货数量\n2、退出入库程序\n";
                    int sss;
                    cin>>sss;
                    if(sss==1) goto FLAGGG;
                    else return;
                }
                this->c_Array[aa].quantity+=hahaha;
                strcpy(this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].brand_name,bbbma);
                this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].brand_code=Bcodee;
                this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].quantity=hahaha;
                this->capacity+=hahaha;//更新仓库已有货物的容量
                this->c_Array[aa].quantity+=this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].quantity;
                cout<<"请输入此品牌商品的价格： ";
                cin>>this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].brnd_price;
                this->WriteInold(aa,this->c_Array[aa].brnd[this->c_Array[aa].nBrnd].quantity,this->c_Array[aa].nBrnd);//写进货清单
                this->c_Array[aa].nBrnd++;
                this->capacity+=hahaha*this->c_Array[aa].ocpycpct;
            }
        }
        else//传新品种
        {
            cout<<"库中无此商品，请先添加！\n";
            this->AddCItem();
            return;
        }
        cout<<"入库成功！\n";
        this->jixu();
    }

    void ReadExitfile() //读取出货清单功能
    {
        ifstream ifs;
        ifs.open("Exitfile.txt",ios::in);
        if(!ifs.is_open())
        {
            cout<<"文件不存在！\n";
            return;
        }
        char buf;
        if(ifs.eof())
        {
            cout<<"文件为空！\n";
            ifs.close();
            return;
        }
        else
        {
            ifs>>noskipws;
            while(!ifs.eof())
            {
                ifs>>buf;
                cout<<buf;
            }
            this->jixu();
            ifs.close();
        }
    }

    void ReadInfile() //读取进货清单功能
    {
        ifstream ifs;
        ifs.open("Infile.txt",ios::in);
        if(!ifs.is_open())
        {
            cout<<"文件不存在！\n";
            return;
        }
        char buf;
        if(ifs.eof())
        {
            cout<<"文件为空！\n";
            ifs.close();
            return;
        }
        else
        {
            ifs>>noskipws;
            while(!ifs.eof())
            {
                ifs>>buf;
                cout<<buf;
            }
            this->jixu();
            ifs.close();
        }
    }

    ~Mangercangku()
    {
        this->save();
    }
};
int main()
{
    while(1)
    {
        Mangercangku m1;
        m1.ShowMenu();
        int select;
        cin>>select;
        switch(select)
        {
        case 1: //添加商品品种
            m1.AddCItem();
            getchar();
            break;
        case 2: //显示商品品种
            m1.ShowCItem();
            getchar();
            break;
        case 3: //查询商品
            m1.SeekItem();
            getchar();
            break;
        case 4: //删除商品
            m1.DeleteItem();
            getchar();
            break;
        case 5: //商品出库
            m1.ItemExit();
            getchar();
            break;
        case 6: //商品进库
            m1.ItemIn();
            getchar();
            break;
        case 7: //查询出货清单
            m1.ReadExitfile();
            getchar();
            break;
        case 8: //查询进货清单
            m1.ReadInfile();
            getchar();
            break;
        case 0: //退出系统
            m1.ExitSystem();
            getchar();
            break;
        }
    }
    return 0;
}