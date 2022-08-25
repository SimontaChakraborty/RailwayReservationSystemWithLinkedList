#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fptr, *fptr2;

struct station{
    char name[50];
    struct station *left;
    struct station *right;
    int leftDistance, rightDistance, stationNo;
    float trainLeavingTime;
    float trainArrivalTime;
    int seatsTaken[2][3];
};
struct train{
    char name[30];
    int ACSeat;
    int shovonSeat;
    int chairSeat;
    float ACprice;
    float shovonPrince;
    float chairPrice;
    struct train *next;
    int trainNo;
};

struct route{
    struct train *tr;
    struct station *start, *end;
}*route1, *route2, *route3;


void addTrain(){
    printf("Select Route: ");
    int route;
    scanf("%d", &route);

    struct train *t = malloc(sizeof(struct train));
    struct route *r;
    if(route == 1){
        r = route1;
    }
    else if(route == 2){
        r = route2;
    }
    else if(route == 3){
        r = route3;
    }
    else {
        printf("Invalid Route\n");
        return;
    }
    printf("Input Train Name: ");
    getchar();
    gets(t->name);
    t->ACprice = 3;
    t->chairPrice = 2;
    t->shovonPrince = 1;
    t->ACSeat = 10;
    t->chairSeat = 10;
    t->shovonSeat = 10;
    t->next = NULL;
    if(r->tr == NULL){
        r->tr = t;
        t->trainNo = 1;
    }
    else{
        struct train *temp = r->tr;
        while(temp->next!=NULL)
            temp = temp->next;
        temp->next = t;
        t->trainNo = temp->trainNo + 1;
    }
}

void addStation(){
    printf("Select Route: ");
    int route;
    scanf("%d", &route);
    struct station *new = malloc(sizeof(struct station));
    struct route *r;
    if(route == 1)
        r = route1;
    else if(route == 2)
        r = route2;
    else if(route == 3)
        r = route3;
    else {
        printf("Invalid Route\n");
        return;
    }

    printf("Input Station Name: ");
    scanf("%s",new->name);

    if(r->start == NULL){
        r->start = new;
        r->end = new;
        new->left = NULL;
        new->right = NULL;
        new->leftDistance = 0;
        new->rightDistance = 0;
        new->stationNo = 1;
        new->seatsTaken[0][0] = new->seatsTaken[0][1] = new->seatsTaken[0][2] = new->seatsTaken[1][0] = new->seatsTaken[1][1] = new->seatsTaken[1][2] = 0;
        switch(route){
            case 1: new->trainLeavingTime = 7.5;
            case 2: new->trainLeavingTime = 5.5;
            case 3: new->trainLeavingTime = 6.5;
        }
        printf("Central Station for Route %d added!\n",route);
    }
    else
    {
        struct station *temp = r->start;
        int distance = 0;
        while (temp->right!=NULL){
            distance += temp->rightDistance;
            temp = temp->right;
        }
        printf("Input Distance From %s: ",temp->name);
        scanf("%d",&temp->rightDistance);
        new->leftDistance = temp->rightDistance;
        new->rightDistance = 0;
        new->seatsTaken[0][0] = new->seatsTaken[0][1] = new->seatsTaken[0][2] = new->seatsTaken[1][0] = new->seatsTaken[1][1] = new->seatsTaken[1][2] = 0;
        temp->right = new;
        new->left = temp;
        new->right = NULL;
        r->end = new;
        new->stationNo = temp->stationNo + 1;
        new->trainLeavingTime = temp->trainLeavingTime+temp->rightDistance*0.3;
    }
    struct station *temp = r->end;
    r->end->trainArrivalTime = r->end->trainLeavingTime + 0.10;
    temp = temp->left;
    while (temp != NULL) {
        temp->trainArrivalTime = temp->right->trainArrivalTime + temp->rightDistance * 0.03;
        temp = temp->left;
    }
}

void add(){
    printf("\n\t1.Add Train\n\t2.Add Station\n");
    int x;
    printf("Enter Your Option: ");
    scanf("%d", &x);
    if(x==1)
        addTrain();
    else if(x==2)
        addStation();
    else
        //printf("Invalid Input\n");
        return;
}

void loadData(){
    struct station *new;
    struct route* r;
    int route;
    fptr=fopen("stations.txt","r");
    while (!feof(fptr)){
        new = malloc(sizeof(struct station));
        fscanf(fptr,"%d %d %d %d %d %d %d %d %d %s\n", &route, &new->leftDistance, &new->rightDistance, &new->seatsTaken[0][0], &new->seatsTaken[0][1], &new->seatsTaken[0][2], &new->seatsTaken[1][0], &new->seatsTaken[1][1], &new->seatsTaken[1][2],  new->name);
        switch (route){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        if(r->start == NULL) {
            r->start = new;
            r->end = new;
            new->left = NULL;
            new->right = NULL;
            new->stationNo = 1;
            switch(route){
                case 1: new->trainLeavingTime = 7.5;
                case 2: new->trainLeavingTime = 5.5;
                case 3: new->trainLeavingTime = 6.5;
            }
        }
        else{
            struct station *temp = r->start;
            while (temp->right!=NULL){
                temp = temp->right;
            }
            temp->right = new;
            new->left = temp;
            new->right = NULL;
            r->end = new;
            new->stationNo = temp->stationNo + 1;
            new->trainLeavingTime = temp->trainLeavingTime + (temp->rightDistance)*0.03;

        }

    }
    for(int i=1; i<=3; i++) {
        switch (i){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        struct station *temp = r->end;
        r->end->trainArrivalTime = r->end->trainLeavingTime + 0.10;
        temp = temp->left;
        while (temp != NULL) {
            temp->trainArrivalTime = temp->right->trainArrivalTime + temp->rightDistance * 0.03;
            temp = temp->left;
        }
    }

    fptr=fopen("trains.txt","r");
    struct train *new2;
    char str1[20], str2[20];
    while (!feof(fptr)){
        new2 = malloc(sizeof(struct train));
        fscanf(fptr,"%d %s %s\n", &route, str1, str2);
        strcat(str1," ");
        strcat(str1,str2);
        strcpy(new2->name, str1);
        switch (route){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        new2->ACprice = 3;
        new2->chairPrice = 2;
        new2->shovonPrince = 1;
        new2->ACSeat = 0;
        new2->chairSeat = 0;
        new2->shovonSeat = 0;
        new2->next = NULL;
        if(r->tr == NULL){
            r->tr = new2;
            new2->trainNo = 1;
        }
        else{
            struct train *temp = r->tr;
            while(temp->next!=NULL)
                temp = temp->next;
            temp->next = new2;
            new2->next=NULL;
            new2->trainNo = temp->trainNo + 1;
        }
    }
    fclose(fptr);

}

void saveData(){
    fptr=fopen("stations.txt","w");
    struct route *r;
    struct station *temp;
    for(int i=1; i<=3; i++){
        switch (i){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        temp = r->start;
        while (temp!=NULL){
            fprintf(fptr, "%d %d %d %d %d %d %d %d %d %s\n", i, temp->leftDistance, temp->rightDistance, temp->seatsTaken[0][0],temp->seatsTaken[0][1],temp->seatsTaken[0][2], temp->seatsTaken[1][0],temp->seatsTaken[1][1],temp->seatsTaken[1][2], temp->name);
            temp=temp->right;
        }
    }
    fclose(fptr);

    fptr=fopen("trains.txt","w");
    struct train *temp2;
    for(int i=1; i<=3; i++){
        switch (i){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        temp2 = r->tr;
        while (temp2!=NULL){
            fprintf(fptr, "%d %s\n", i, temp2->name);
            temp2=temp2->next;
        }
    }
    fclose(fptr);
}


void profile(){
    fptr=fopen("pass.txt","r");
    char res[500];
    while(fgets(res, 500, (FILE *)fptr))
    {
        printf("%s\n", res);
    }
    fclose(fptr);
}

void admin(){
    char username[10];
    int password,c=0;
    printf("Enter Your UserName: ");
    scanf("%s",&username);
    printf("Enter Your PassWord: ");
    scanf("%d",&password);

    if(strcmp(username,"Simonta")==0){
        if(password==2018){
            printf("\n.....Log In Successful.....\n");
            c=1;
        }

        else
            printf("\n....Invalid Password....\n");
    }
    else
        printf("\n....Invalid User....\n");
    while(c==1){
        printf("\n\t1.User Info\n\t2.Add\n\t3.Save changes\n\t4.Previous menu\n");
        int z;
        printf("\n\tEnter Option: ");
        scanf("%d",&z);
        if(z==1)
            profile();
        else if(z==2)
            add();
        else if(z==3)
            saveData();
        else if(z==4)
            break;
        else
            printf("\n....Wrong Input....\n");
    }
}

void signup(){
    char username[100],mobileNo[100];
    int password;

    fptr=fopen("pass.txt","a");

    if(fptr==NULL){
        printf("File Does not Exist.. \n");
        return;
    }
    printf("\n\n");
    printf("Enter Your UserName: ");
    scanf("%s",&username);
    fprintf(fptr,"%s ", username);

    printf("Enter Your Password: ");
    scanf("%d",&password);
    fprintf(fptr,"%d ",password);

    printf("Enter Your Mobile Number: ");
    scanf("%s",&mobileNo);
    fprintf(fptr,"%s\n",mobileNo);

    printf("\n....Registration Successful....\n");
    fclose(fptr);
}

float changeTime(float hour){
    int h = (int)hour;
    float decimal = hour-h;
    decimal = decimal*60/100.0;
    if(h>24)
        h = h-24;
    return (h+decimal);
}

void ticket(){
    printf("\n\tSelect Route\n"
           "\n\t1.%s to %s\n"
           "\n\t2.%s to %s\n"
           "\n\t3.%s to %s\n", route1->start->name,route1->end->name, route2->start->name,route2->end->name,route3->start->name,route3->end->name);
           int route, start, end,opt, distance = 0;
           printf("Select Your Route: ");
           scanf("%d", &route);
           struct route *r;
           switch (route){
            case 1: r = route1;
                break;
            case 2: r = route2;
                break;
            case 3: r = route3;
                break;
        }
        printf("\n...Available Stations...\n");
        struct station *temp = r->start, *startingSt, *endingSt;
        while(temp!=NULL){
            printf("%d. %s\n",temp->stationNo, temp->name);
            temp = temp->right;
        }
        printf("Select starting station: \n");
        scanf("%d",&start);
        printf("\n...Available Stations...\n");
        temp = r->start;
        while(temp!=NULL){
            printf("%d. %s\n",temp->stationNo, temp->name);
            temp = temp->right;
        }
        printf("\nSelect destination station: \n");
        scanf("%d",&end);
        if(start<end){
            temp = r->start;
            while(temp->stationNo!=start)
                temp = temp->right;
            startingSt = temp;
            while(temp->stationNo!=end){
                distance+= temp->rightDistance;
                temp = temp->right;
            }
            endingSt = temp;
            printf("Distance from %s to %s = %d km\n", startingSt->name, endingSt->name ,distance);
        }
        else if(start>end){
            temp = r->end;
            while(temp->stationNo!=start)
                temp = temp->left;
            startingSt = temp;
            while(temp->stationNo!=end){
                distance+= temp->leftDistance;
                temp = temp->left;
            }
            endingSt = temp;
            printf("Distance from %s to %s = %d km\n", startingSt->name, endingSt->name ,distance);
        }
        else{
            printf("INVALID INPUT!...\n SOURCE AND DESTINATION CANNOT BE SAME. PLEASE REVISIT YOUR OPTIONS AND FIX YOUR INPUT\n\n\n");
            return;
        }
        printf("Select Train: \n");
        struct train *temp2 = r->tr;
        while(temp2!=NULL){
            printf("%d. %s \n", temp2->trainNo, temp2->name);
            temp2= temp2->next;
        }
        printf("Select option: ");
        scanf("%d",&opt);
        temp2 = r->tr;
        while(temp2->trainNo!=opt){
            temp2= temp2->next;
        }
        int class;
        printf("\nSelect Class: \n"
               "1. AC Seat\n"
               "2. Chair Seat\n"
               "3. Shovon\n");
        printf("Enter Your Option: ");
        scanf("%d",&class);
            printf("Number Of seats: ");
            scanf("%d",&opt);
            if(opt<1 || opt>4){
                printf("You cannot buy %d numbers of ticket\n",opt);
                return;
            }
            else{
                float p= (4-class)*distance;
                float price = (4-class)*distance*opt;

                printf("Price of 1 Ticket = %3.2f\n",p);
                printf("Total Ticket Price = %3.2f\n",price);
                temp = startingSt;
                if(start<end){
                    while(temp!=endingSt){
                        if(temp->seatsTaken[0][class-1] + opt> 10 )
                        {
                            printf("Error! Seats not available\n");
                            return;
                        }
                        temp = temp->right;
                    }
                }
                else{
                    while(temp!=endingSt){
                        if(temp->seatsTaken[1][class-1] + opt> 10 )
                        {
                            printf("Error! Seats not available\n");
                            return;
                        }
                        temp = temp->left;
                    }
                }

                printf("Confirm purchase? \n1.Yes\n2.No \n ");
                int x;
                scanf(" %d",&x);
                if(x==1){
                    if(start<end){
                        temp = startingSt;
                        while(temp!=endingSt){
                            temp->seatsTaken[0][class-1] = temp->seatsTaken[0][class-1] + opt;
                            temp = temp->right;
                        }
                    }
                    else{
                        temp = startingSt;
                        while(temp!=endingSt){
                            temp->seatsTaken[1][class-1] = temp->seatsTaken[1][class-1] + opt;
                            temp = temp->left;
                        }

                    }
                    printf("Ticket Purchase Successful\n");
                    printf("From: %s \t\t To: %s\n"
                           "No of seats: %d\n", startingSt->name, endingSt->name,opt);
                    printf("Price of 1 Ticket = %3.2f\n",p);
                    printf("Total Ticket Price = %3.2f\n",price);

                    if(start>end){
                        printf("Departure Time: %2.2f\t Arrival Time: %2.2f\n",changeTime(startingSt->trainArrivalTime), changeTime(endingSt->trainArrivalTime));
                    }
                    else{
                        printf("Departure Time: %2.2f\t Arrival Time: %2.2f\n",changeTime(startingSt->trainLeavingTime), changeTime(endingSt->trainLeavingTime));
                    }
                    saveData();
                }
                else if(x==2){
                    printf("Ticket Purchase cancelled");
                    return;
                }
                else{
                    printf("\n....Wrong Input.....\n");
                    return;
                }
            }
}

void purchase(){

    int x;
    while (1){
        printf("\n\t...Welcome To The Railway Ticket Reservation System...\n"
               "\t1.Buy Tickets\n"
               "\t2.Return to previous menu\n");
        printf("Enter Your Option: ");
        scanf("%d",&x);;
        if(x==1)
            ticket();
        else if(x==2)
            break;
        else
            printf("\n....Wrong Input!....");
    }
}

void SignIn2(){
    int i, size=0;
    char user [20];
    int upass,num;
    char name [100][20];
    int pass[100];
    fptr2 = fopen("pass.txt","r");
    for(i=0; !feof(fptr2); i++){
        fscanf(fptr2,"%s %d %s",name[i],&pass[i],&num);
        size++;
    }
    fclose(fptr2);
    printf("Input Username: ");
    scanf("%s",user);
    printf("Input Password: ");
    scanf("%d",&upass);
    for(i=0;i<size;i++){
        if(strcmp(user,name[i])==0){
            break;
        }
    }
    if(strcmp(user,name[i])==0){
        if(pass[i] == upass) {
            printf("\n.....Log In Successful.....\n");
            purchase();
        }
        else
            printf("\n....Invalid Password....\n");
        }
    else
        printf("\n....Invalid User....\n");
}

void signin(){

    while(1){
        printf("\n\t1.User Sign In\n\t2.Back to Main Menu\n");
        int x;
        printf("\nEnter Option: ");
        scanf("%d",&x);
        if(x==1)
            SignIn2();
        else if(x==2)
            break;
        else
            printf("\n....Wrong Input....\n");
    }
}

int main()
{

    route1 = malloc(sizeof(struct route));
    route2 = malloc(sizeof(struct route));
    route3 = malloc(sizeof(struct route));
    route1->start = route1->end = route2->start = route2->end = route3->start = route3->end = NULL;
    route1->tr = route2->tr = route3->tr = NULL;
    loadData();


    while(1){
        printf("\t\n......Welcome to Our Train Ticket Reservation System......\n\n");
        printf("\n\t1.Admin Login\n\t2.User Sign Up\n\t3.User Sign In\n\t4.Exit\n");
        int n;
        printf("\nEnter Option: ");
        scanf("%d",&n);
        if(n==1)
            admin();
        else if(n==2)
            signup();
        else if(n==3)
            signin();
        else if(n==4){
            printf("...Thanks For Using Our System...\n\n");
            break;
        }
        else
            printf("\n....Wrong Input....\n");
    }
}
