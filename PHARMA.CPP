#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
int inum, snum;//variables to store total number of items, suppliers

int gdriver, gmode;
struct itemtype
{
	int itemnum;
	char chemname[21];
	int stockqty;
	char billdate[11];
	char manudate[11];
	char expdate[11];
	char prescription;//'y' if it is a prescription drug and 'n' if it isn't
	float cost;
	int minqty;
	int orderqty;
	int supnum;
	int flag1;
	//deletion of record : flag=1 for existing record and flag=0 for deleted record
};
itemtype item;

struct suppliertype
{
	int supno;
	char supname[41];
	char supaddress[41];
	char sup_phone[11];
};
suppliertype supp;
struct numbers//this will be updated every time an item/supplier is added
{
	int inumber;
	int snumber;
}n;

fstream invfile,supfile,porder,nfile,bill;


int increase_items()
{
	nfile.open("num.dat",ios::binary|ios::in|ios::out);
	nfile.read((char*)&n, sizeof(n));
	n.inumber=n.inumber+1;
	nfile.write((char*)&n, sizeof(n));
	nfile.close();
	return n.inumber;
}
int increase_suppliers()
{
	nfile.open("num.dat",ios::binary|ios::in|ios::out);
	nfile.read((char*)&n, sizeof(n));
	n.snumber=n.snumber + 1;
	nfile.write((char*)&n, sizeof(n));
	nfile.close();
	return n.snumber;
}

void welcome()
{
	gdriver=DETECT;  //requesting auto detection
	initgraph(&gdriver,&gmode,"C:\\TURBOC3\\BGI");//initialising graph
	int errorcode = graphresult();
	if (errorcode != grOk)  //error occurred
	{
		cout<<"Graphics error: "<< grapherrormsg(errorcode)<<endl;
		cout<<"Press enter to halt:";
		cin.get();
		return;
	}
	int x,y,i;
	x=getmaxx()/2;
	y=getmaxx()/2;
	for(i=30;i<200;i++)
	{	delay(30);
		setcolor(i/10);
		arc(x,y,0,180,i-10);
		setbkcolor(BLACK);
	}
	cin.get();
	cleardevice();
	closegraph();
	gdriver=DETECT;
	initgraph(&gdriver,&gmode,"C:\\TURBOC3\\BGI");
	setbkcolor(RED);
	setcolor(14);
	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,8);
	settextstyle(8,HORIZ_DIR,8);
	moveto(90,100);
	outtext("PHARMACY");
	moveto(45,210);
	outtext("MANAGEMENT");
	setcolor(15);
	settextstyle(8,HORIZ_DIR,1);
	cin.get();
	moveto(200,200);
	outtext("Computer Science Project");
	moveto(185,260);
	cin.get();
	closegraph();  //closing graphics screen
}

void draw_border()
{
	cleardevice();
	for(int i=2;i<79;i++)//drawing horizontal lines
	{
		gotoxy(i,2);
		cout<<".";
		gotoxy(i,25);
		cout<<".";
	}
	for(i=4;i<=36;i++)//drawing vertical lines
	{
		gotoxy(2,i);
		cout<<".";
		gotoxy(78,i);
		cout<<".";
	}
}



class inventory
{
	private:

	void generate_purchase_order()
	{
		suppliertype r;
		if(item.flag1!=0)
		{
			if(item.stockqty <= item.minqty)
			{
				r=search_supplier(item.supnum);
				char filename[36];
				strcpy(filename,strcat(item.chemname,".txt"));
				porder.open(filename, ios::out);
				if(!porder) cerr<<"\n not opened";
				char companyname[80];
				strcpy(companyname,"Pharma co.");
				porder<<companyname<<endl;
				porder<<endl<<endl<<"The Proprietor"<<endl<<companyname<<endl<<endl;
				porder<<"Manager"<<endl<<r.supname<<endl<<r.supaddress<<endl<<endl;
				porder<<"Dear Sir"<<endl<<"Subject: Purchase order for "<<item.chemname<<endl;
				porder<<"I am the proprietor of "<<companyname<<"."<<endl;
				porder<<"We have run out of stock of "<<item.chemname<<" and we wish to place an order."<<endl;
				porder<<"The details are as follows :"<<endl;
				porder<<"\t NAME : "<<item.chemname<<"\n\t QUANTITY : "<<item.orderqty<<endl;
				porder<<"We kindly request you to deliver the order within five working "<<endl;
				porder<<"days from today at the standard wholesale price."<<endl<<endl;
				porder<<"The payment will be made by cheque in favour of "<<supp.supname<<endl;
				porder<<endl<<endl<<"Thanking You."<<endl<<"Yours sincerely,"<<endl<<"Proprietor"<<endl<<companyname;
				draw_border();
				gotoxy(6,6);
				cout<<"\n Purchase order generated for"<<item.chemname;
				cin.get();
				porder.close();
			}
		}
	}
	public:
	itemtype search_item(char s[21])
	{
		invfile.open("invfile.dat" ,ios::binary|ios::in); //opening the file
		invfile.read((char*)&item,sizeof(item));//priming read
		while(!invfile.eof())
		{
			if(item.flag1!=0)
			{
				if (!strcmp(item.chemname, s))
				{
					invfile.close();
					return item;   //returning the searched record
				}
			}
			invfile.read((char*)&item,sizeof(item));//reading from file

		}
		invfile.close();
		item.itemnum=-1;
		return item;
	}
	itemtype search_item(int n)
	{
		invfile.open("invfile.dat" ,ios::binary|ios::in);
		invfile.read((char*)&item,sizeof(item));//priming read
		while(!invfile.eof())
		{
			if(item.flag1!=0)
			{
				if (n==item.itemnum)
				{
					invfile.close();
					return item;   //returning the searched record
				}
			}
			invfile.read((char*)&item,sizeof(item));//reading from file
		}
		invfile.close();
		item.itemnum=-1;
		return item;
	}

	suppliertype search_supplier(int num)
	{
		supfile.open("supplier.dat" ,ios::binary|ios::in);
		supfile.read((char*)&supp,sizeof(supp));//priming read
		while(!supfile.eof())
		{

			if (supp.supno==num)
			{
				supfile.close();
				return supp;   //returning the searched record
			}
			supfile.read((char*)&supp,sizeof(supp));//reading from file

		}
		supfile.close();
		supp.supno=0;
		return supp;
	}

	void add_item_to_file()
	{

		char reply;
		invfile.open("invfile.dat" ,ios::binary|ios::app);
		if(!invfile)cerr<<"\n not opened";
		do
		{
			clrscr();
			draw_border();
			item.itemnum=inum;
			inum=inum+1;
			gotoxy(28,1);
			cout<<"ENTER DETAILS ";
			gotoxy(15,4);
			cout<<"Number -";
			gotoxy(25,4);
			cout<<item.itemnum;
			gotoxy(15,5);
			cout<<"Name : ";
			cin.getline(item.chemname,21); cin.get();
			gotoxy(15,6);
			cout<<"Stock : ";
			cin>>item.stockqty; cin.get();
			gotoxy(15,7);
			cout<<"Date of order : ";
			cin.getline(item.billdate,11); cin.get();
			gotoxy(15,8);
			cout<<"Date of manufacture : ";
			cin.getline(item.manudate,11); cin.get();
			gotoxy(15,9);
			cout<<"Expiry date : ";
			cin.getline(item.expdate,11); cin.get();
			gotoxy(15,10);
			cout<<"Rate (Rs.):";
			cin>>item.cost; cin.get();
			gotoxy(15,11);
			cout<<"Prescription drug (y/n):";
			cin>>item.prescription; cin.get();
			gotoxy(15,12);
			cout<<"Minimum quantity for generating purchase order : ";
			cin>>item.minqty; cin.get();
			gotoxy(15,13);
			cout<<"Usual order quantity : ";
			cin>>item.orderqty; cin.get();
			gotoxy(15,14);
			cout<<"Enter supplier code : ";
			cin>>item.supnum; cin.get();
			gotoxy(25,16);
			item.flag1=1;
			invfile.write((char*)&item, sizeof(item));
			gotoxy(15,15);
			cout<<"Do you want to enter more (Y/N) ";
			cin>>reply;cin.get();

		} while (tolower(reply)=='y');
		invfile.close();
		invfile.open("invfile.dat", ios::binary|ios::in);
		cleardevice();
		invfile.close();
	}

	void disp_all_items()
	{
		cleardevice();

		gotoxy(6,0);
		invfile.open("invfile.dat", ios::binary|ios::in);
		invfile.read((char*)&item, sizeof(item));//priming read
		while(!invfile.eof())
		{
			if(item.flag1!=0)
			{
				cout<<"\n "<<item.itemnum<<setw(15)<<item.chemname
					<<setw(4)<<item.stockqty<<setw(4)<<item.cost;
			}
			invfile.read((char*)&item, sizeof(item));
		}
		invfile.close();
	}

	void disp_item_search()
	{
		char reply='y';
		do
		{
			cleardevice();
			draw_border();
			gotoxy(28,2);
			cout<<"~~SEARCH RECORD~~";
			gotoxy(15,6);
			cout<<"Enter product name : ";
			char name[21];
			cin.getline(name,21);
			item=search_item(name); //calling for search in invfile
			// displaying records
			if (item.itemnum!=-1)
			{
				gotoxy(18,9);
				cout<<"The record is";
				gotoxy(20,10);
				cout<<" Number :";
				cout<<item.itemnum;
				gotoxy(20,11);
				cout<<" Stock :";
				cout<<item.stockqty;
				gotoxy(20,12);
				cout<<" Name : ";
				cout<<item.chemname;
				gotoxy(20,13);
				cout<<" Rate :(Rs.)";
				cout<<item.cost;
			}
			else
			{
				gotoxy(20,10);
				cout<<"Sorry. The requested item was not found";
			}
			gotoxy(15,16);
			cout<<"Do you wish to search for another item (Y/N)";
			cin>>reply; cin.get();
		} while(tolower(reply)=='y');
	}

	void add_supp_to_file()
	{

		char reply;
		supfile.open("supplier.dat" ,ios::binary|ios::app);
		if(!supfile)cerr<<"\n not opened";

		do
		{
			cleardevice();
			draw_border();
			supp.supno=snum;
			snum=snum+1;
			textcolor(LIGHTBLUE);
			gotoxy(28,1);
			cout<<"ENTER DETAILS OF SUPPLIER";
			gotoxy(15,4);
			cout<<"Number -";
			gotoxy(25,4);
			cout<<supp.supno;
			gotoxy(15,5);
			cout<<"Name : ";
			cin.getline(supp.supname,41); cin.get();
			//if (!item.chemname[0])           //to undo entering if name is not entered
			       //return;
			gotoxy(15,6);
			cout<<"Address : ";
			cin.getline(supp.supaddress,41); cin.get();
			gotoxy(15,7);
			cout<<"Phone Number";
			cin.getline(supp.sup_phone,11); cin.get();
			supfile.write((char*)&supp, sizeof(supp));
			gotoxy(15,10);
			cout<<"Do you want to enter more (Y/N) ";
			cin>>reply;cin.get();
		} while (tolower(reply)=='y');
		supfile.close();
		supfile.open("supplier.dat", ios::binary|ios::in);
		cleardevice();
		supfile.close();
	}

	void disp_all_suppliers()
	{
		cleardevice();
		draw_border();
		gotoxy(6,0);
		supfile.open("suppliers.dat", ios::binary|ios::in);
		supfile.read((char*)&supp, sizeof(supp));//priming read
		while(!supfile.eof())
		{
			cout<<"\n "<<supp.supno<<setw(25)<<supp.supname
				<<"  "<<supp.supaddress<<"\n\t\t"<<supp.sup_phone;
			supfile.read((char*)&supp, sizeof(supp));
		}
		supfile.close();
	}

	void check_stock()
	{
		invfile.open("invfile.dat", ios::binary|ios::in);
		invfile.read((char*)&item, sizeof(item));//priming read
		while(!invfile.eof())
		{
			if(item.flag1!=0)
			{
				generate_purchase_order();
			}
			invfile.read((char*)&item, sizeof(item));
		}
		invfile.close();
	}
	void update_inventory()
	{
		char s[21];
		char reply;
		do
		{
			cleardevice();
			draw_border();
			gotoxy(15,5);
			cout<<"\n Item name :";
			cin.getline(s,21);
			long medno=0;
			invfile.open("invfile.dat", ios::binary|ios::in|ios::out);
			invfile.read((char*)&item, sizeof(item));//priming read
			while(!invfile.eof())
			{
				medno++;
				if(item.flag1!=0)
				{
					if(!strcmp(item.chemname, s))
					{
						item.stockqty=item.stockqty + item.orderqty;
						gotoxy(15,10);
						cout<<"New Stock quantity : "<<item.stockqty;
						gotoxy(15,11);
						cout<<"Order Date : ";
						cin.getline(item.billdate,11);cin.get();
						gotoxy(15,12);
						cout<<"Manufacture Date : ";
						cin.getline(item.manudate,11); cin.get();
						gotoxy(15,13);
						cout<<"Expiry date : ";
						cin.getline(item.expdate,11);cin.get();
						invfile.seekp((medno-1)*sizeof(item), ios::beg);
						invfile.write((char*)&item, sizeof(item));
					}
				}
				invfile.read((char*)&item, sizeof(item));
			}
			invfile.close();
			gotoxy(15,15);
			cout<<"Do you wish to update after another delivery?";
			cin>>reply; cin.get();
		}while(tolower(reply)!='n');
	}

	void delete_item()
	{
		cleardevice();
		char s[21];
		cout<<"Name of medicine to be deleted :";
		cin.getline(s,21);
		long recno=0;
		invfile.open("invfile.dat", ios::binary|ios::in|ios::out);
		invfile.read((char*)&item, sizeof(item));//priming read
		while(!invfile.eof())
		{
			recno++;
			if(!strcmp(item.chemname,s))
			{
				item.flag1=0;
				invfile.seekp((recno-1)*sizeof(item), ios::beg);
				invfile.write((char*)&item, sizeof(item));
			}
			invfile.read((char*)&item, sizeof(item));
		}
		gotoxy(15,15);
		cout<<endl<<s<<" DELETED ";
		invfile.close();
	}


};
class billing
{
	private:
	inventory y;
	itemtype w;
	/*
	Every time an item has been billed, the inventory contents change and
	the stock reduces. The following function is responsible for updating inventory
	during the billing process.
	*/
	void update_stock(int n, int q)//n - item number, q - quantity
	{
		invfile.open("invfile.dat" ,ios::binary|ios::in|ios::out);
		long recno=0;
		invfile.read((char*)&item,sizeof(item));//priming read
		while(!invfile.eof())
		{
			recno++;
			if (n==item.itemnum)
			{
				item.stockqty=item.stockqty-q;
				invfile.seekp((recno-1)*sizeof(item), ios::beg);
				invfile.write((char*)&item,sizeof(item));
			}
			invfile.read((char*)&item,sizeof(item));
		}
		invfile.close();
	}
	void screen()
	{
		cleardevice();

		gotoxy(6,5);
		cout<<"S.No";
		gotoxy(11,5);
		cout<<"I.No";
		gotoxy(22,5);
		cout<<"Product Name";
		gotoxy(40,5);
		cout<<"Rate";
		gotoxy(46,5);
		cout<<"Quantity";
		gotoxy(60,5);
		cout<<"Price";

		for (int i=2;i<79;i++)//drawing horizontal lines
		{
			gotoxy(i,7);
			cout<<".";
		}
		for (i=4;i<=36;i++)//drawing vertical lines
		{
			gotoxy(5,i);
			cout<<".";
			gotoxy(10,i);
			cout<<".";
			gotoxy(16,i);
			cout<<".";
			gotoxy(38,i);
			cout<<".";
			gotoxy(44,i);
			cout<<".";
			gotoxy(55,i);
			cout<<".";
			gotoxy(68,i);
			cout<<".";
		}
	}
	public:
	void disp_bill()
	{
		inventory c;
		screen();
		int no=0, qty=0;
		float price=0, total=0;
		int z;
		bill.open("Bill.txt", ios::out);
		bill<<"\t\t\tPHARMA CO.\n\t\t\t\tBill\n";
		bill<<"S.No |Product Name \t |Rate   |Qty    |Price   "<<endl;
		cout<<"\nENTER -1 to terminate";
		for(z=1; no!=(-1); ++z)
		{
			gotoxy(6,6+z);
			cout<<z;
			gotoxy(11,6+z);
			cin>>no; cin.get();
			if(no!=-1)
			{
				gotoxy(22,6+z);
				w=y.search_item(no);
				if(w.itemnum!=-1)
				{
					cout<<w.chemname;
					gotoxy(40,6+z);
					cout<<w.cost;
					gotoxy(46,6+z);
					cin>>qty;cin.get();
					if(qty>=(w.stockqty))
					{
						gotoxy(49,6+z);
						cout<<"!";//symbol will mean we are deficient in stock
						int option;
						/*
						if customer wishes, customer will get whatever
						maximum stock we have. else nothing.
						*/
						cin>>option; cin.get();
						if(option!=0)qty=w.stockqty;
						else qty=0;
					}
					gotoxy(60,6+z);
					price=w.cost*float(qty);
					cout<<setprecision(2);
					cout<<price;
					total=total+price;
					update_stock(w.itemnum, qty);
				}
				c.check_stock();
			}
			bill<<"   "<<z<<". "<<w.chemname<<"  "<<w.cost<<"  "<<qty<<"   "<<price<<endl;
		}
		gotoxy(42,8+z);
		cout<<"Total Price : Rs. "<<total;
		bill<<endl<<"\t\t\t\tTOTAL PRICE : "<<total;
		bill.close();
	}

};

void menu()
{
	int n;
	inventory inv;
	billing bil;
	int tru=1;
	do
	{       gdriver=DETECT;
		initgraph(&gdriver,&gmode,"C:\\TURBOC3\\BGI");
		cleardevice();
		setbkcolor(2);
		textcolor(YELLOW);
		draw_border();
		textcolor(YELLOW);
		gotoxy(32,5);
		cout<<"WELCOME";
		for (int z=29;z<=40;z++)
		{
			gotoxy(z,6);
			cout<<"-";
		}
		gotoxy(20,7);
		cout<<"1) Add a new medicine";
		gotoxy(20,8);
		cout<<"2) Add a new supplier";
		gotoxy(20,9);
		cout<<"3) Display inventory contents";
		gotoxy(20,10);
		cout<<"4) Display supplier list";
		gotoxy(20,11);
		cout<<"5) Search inventory ";
		gotoxy(20,12);
		cout<<"6) Generate Bill";
		gotoxy(20,13);
		cout<<"7) Update after delivery";
		gotoxy(20,14);
		cout<<"8) Delete item from inventory";
		gotoxy(20,15);
		cout<<"0) EXIT";
		gotoxy(20,17);
		cout<<"Please enter your choice:";
		cin>>n; cin.get();
		if(n!=0)
		{
			if(n==1) {inv.add_item_to_file(); cin.get();}
			if(n==2) {inv.add_supp_to_file(); cin.get();}
			if(n==3) {inv.disp_all_items(); cin.get();}
			if(n==4) {inv.disp_all_suppliers(); cin.get();}
			if(n==5) {inv.disp_item_search(); cin.get();}
			if(n==6) {bil.disp_bill(); cin.get();}
			if(n==7) {inv.update_inventory(); cin.get();}
			if(n==8) {inv.delete_item(); cin.get();}
		}
		else tru=0;
	} while (tru==1);
	cleardevice();
	closegraph();
}


void main()
{
	n.inumber=1;n.snumber=1;
	nfile.open("num.dat", ios::binary|ios::in);
	nfile.read((char*)&n, sizeof(n));
	inum=n.inumber;
	snum=n.snumber;
	nfile.close();
	welcome();
	menu();
	nfile.open("num.dat", ios::binary|ios::out);
	n.inumber=inum;
	n.snumber=snum;
	nfile.write((char*)&n, sizeof(n));
	getch();
}
