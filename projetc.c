#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>


// Strutures (cellule Cell and chained_list)



struct cellule {
    char* str;
    struct cellule *next;
};
typedef struct cellule Cell;

struct chained_list{
    Cell* first;      //points on the first element of the list
    int length;
    Cell* end;        //points on the last element of the list
};
typedef struct chained_list chained_list;

void initialisation(chained_list* list){
    list->first=NULL;
    list->length=0;
}

void insertion(chained_list* list,char* line){
    Cell* c=malloc(sizeof(Cell));     //we need to free it after getting the line in filtering()
    c->str=strdup(line);
    if(list->first==NULL){
        c->next=NULL;
        list->first=c;
        list->end=c;
    }
    else{
    c->next=NULL;
    (list->end)->next=c;
    list->end=c;
    }
}

void destroy(chained_list* list){
    if(list->first==NULL){
        free(list);
    }
    else{
        Cell* c=list->first;
        Cell* c_1=c;
        while (c!=NULL){
            c=c_1->next;
            free(c_1);
            c_1=c;
        }
        free(list);
    }
}

void show(chained_list* LIST) {
    if(LIST==NULL){
        printf("The initial file is empty!\n");
        exit(EXIT_FAILURE);
    }
    Cell* element=LIST->first;
    while (element){
        printf("%s\n", element->str);
        element = element->next;
    }
    printf("NULL\n");
}

int opening_test(char* file_name){
    FILE* file=NULL;
    file=fopen(file_name,"r"); //test if the file exists
    if (file==NULL){
        fclose(file);
        return -1;               //return -1 if opening error (file doesn't exist)
    }
    else{
        fclose(file);
        return 0;             //return 0 if opening succeeded
    }
}

int count_lines(char* file_name){  //returns the number of lines in a file
    if(opening_test(file_name)==-1){
        printf("FILE DOES NOT EXIST!\n");
        return -1;  //error opening so we stop here
    }
    else{
        FILE* file=NULL;
        file=fopen(file_name,"r");
        int count=0;
        int k=0;
        char current_charac;
        while ((current_charac=fgetc(file))!=EOF){
            k++;
            if(current_charac=='\n'){
                count++;
            }
        }
        if(!k){
            return -1;   //we return -1 because the file is empty
        }
        fseek(file,-1,SEEK_CUR); //we move the cursor backwards for 1 charac (the cursor was poiting on EOF)
        if ((current_charac=fgetc(file))!='\n'){
            count++;              //we count the last line if the user did not jump a line at the end
        }
        fclose(file);
        //printf("Number of lines in %s:%d\n",file_name,count);
        return count;
    }
}




// Step 1 : Pre-treatement




char* str_without_comments(char* string){
    if(strstr(string,"//")==NULL){            //if there is no comments in the str: return it
        return string;
    }
    char last_charac=string[strlen(string)-1];
    char *result=NULL;                         //the function return the entered string without its comments
    result=malloc(sizeof(char)*strlen(string)+1); //we will free in filtering()
    for(int i=0;i<strlen(string);i++){
        if(string[i]=='/'){
            if((i+1!=strlen(string))&&(string[i+1]=='/')){  //we look if there is two consecutives //
                if(last_charac=='\n'){
                    result[i]='\n';         //we need to put the \n if there is one
                    result[i+1]='\0';           //we do not forget to end th str
                }
                else{
                    result[i]='\0';     //we do not forget to end th str
                }
                break;
            }
        }
        else{
            result[i]=string[i];                //we copy while there is no comments
        }
    }
    return result;
}

char* str_without_spaces_tabs(char* string){ //return the string without tabs and spaces
    char* result=NULL;
    result=malloc(sizeof(char)*strlen(string)+1);  //free after usage in filtering
    int k=0;
    for(int i=0;i<strlen(string);i++){
        if((string[i]!=' ')&&(string[i]!='\t')){
            result[k]=string[i];
            result[k+1]='\0';
            k++;
        }
    }
    result[k]='\0';
    return result;
}

char* str_without_strings(char* string){   //function that takes substrings of a string
    char* result=NULL;
    result=malloc(sizeof(char)*strlen(string)+1);//we will free after usage in filtering
    int k=0;
    int flag=0;                 //  FLAG=0 : NOT IN COMMENT ; FLAG=1: IN DOUBLE COMMA COMMENT; FLAG=2: IN SIMPLE COMMA COMMENT
    for(int i=0;i<strlen(string);i++){
        if((string[i]!=34)&&(string[i]!=39)&&(flag==0)){   //we are not in ""  34: code ascii "   39: code ascii '
            result[k++]=string[i];
            //printf("%d\n",flag);
            //printf("%c\n",result[k]);
            result[k]='\0';
            flag=0;
        }
        else if((string[i]==34)&&(flag)==0){ // we encounter the first "
            //printf("on rentre1 pour i=%d\n",i);
            flag=1;
            result[k]=34;   //we put back the "
            result[k+1]='\0';
            k++;
        }
        else if((string[i]==39)&&(flag==0)){ //we encounter the first '
            flag=2;
            result[k]=39;  //we put back the '
            result[k+1]='\0';
            k++;

        }
        else if(((string[i]==34)&&(flag==1)&&(string[i-1]!=92))||((string[i]==39)&&(flag==2)&&(string[i-1]!=92))){ //we go off the string if there is no \' or \"
            if(flag==1){
                result[k]=34;
                result[k+1]='\0';
                k++;
            }
            if(flag==2){
                result[k]=39;
                result[k+1]='\0';
                k++;
            }
            flag=0;
        }
    }
    return result;
}

char* words_to_w(char* string){
    char* result=NULL;
    int k=0;
    result=malloc(sizeof(char)*strlen(string)+1);
    for(int i=0;i<strlen(string);i++){
        if(string[i]<48 || ((string[i]<65)&&(string[i]>57)) ||((string[i]<97)&&(string[i]>90))||((128>string[i])&&(string[i]>122))){  //not a word we rewrite
            result[k]=string[i];
            result[k+1]='\0';
            k++;
        }
        else{
            if(i!=0){
                if(string[i-1]<48 || ((string[i-1]<65)&&(string[i-1]>57)) ||((string[i-1]<97)&&(string[i-1]>90))||((128>string[i-1])&&(string[i-1]>122))){   //if its the first charac of a substring
                    result[k]='w';
                    result[k+1]='\0';
                    k++;
                }
            }
            else{    //its the first charac of the string and its a letter/number
                result[k]='w';
                result[k+1]='\0';
                k++;
            }
        }
    }
    result[k]='\0';
    return result;
}




int filtering(char* file_name_e,char* file_name_o,chained_list* list){ //will write a new filtered file and takes a file in parameter and a list
    FILE* new_file=NULL;
    new_file=fopen(file_name_o,"w");
    int char_max_line=200;
    int number_of_lines=count_lines(file_name_e); //we count the number of lines in the file
    FILE* file=NULL;
    file=fopen(file_name_e,"r"); //read
    char* current_line=NULL;
    char* new_line=NULL;
    //char* ptr=NULL;
    current_line=malloc(char_max_line);
    for(int i=0;i<number_of_lines;i++){ //we look at every line of the file
        long cursor_position_before=ftell(file); //we save the position of the cursor when it is at the beginning of the line
        fgets(current_line,char_max_line,file);
        while((strlen(current_line)==char_max_line-1) && (current_line[char_max_line-2]!='\n')){ //while the line does not fit entierely in char_max_line, we double that value
            current_line=realloc(current_line,char_max_line*2);
            char_max_line*=2;
            fseek(file,cursor_position_before-ftell(file),SEEK_CUR);
            fgets(current_line,char_max_line,file);
        }
        if(current_line[0]=='\n'){    //if there is a blank jump of line, we go to the next iteration
            continue;
        }
        new_line=str_without_comments(current_line); //we delete comments
        if((strstr(current_line,"'")!=NULL)||(strstr(current_line,"\"")!=NULL)){//we look if there is a string in the line
            new_line=str_without_strings(new_line);         //we delete strings
        }
        new_line=words_to_w(new_line);       //we transform words to w
        new_line=str_without_spaces_tabs(new_line);      //we delete spaces and tabulations
        if(new_line[0]=='\n'){
            continue;
        }
        insertion(list,new_line);
        list->length++;
        fputs(new_line,new_file);  //we write the filtrated line in the new file
        free(new_line);             //we free the allocated memory in the 4 functions
    }
    fclose(file);
    fclose(new_file);
    return 0;  //no error
}



// Step 2 : similarity between segments


   // Dice distance


int number_of_digrammes(char* str1){
    int n=strlen(str1);
    if(n<2){
        return 0;
    }
    else{
        if(str1[n-1]=='\n'){
            return n-2;
        }
        else{
            return n-1;
        }
    }
}

int compare_digrammes(char* str1,char *str2){
    int max;
    int n1=number_of_digrammes(str1);    //number of digrammes in str1
    int n2=number_of_digrammes(str2);     //number of digrammes in str2
    int count=0;
    if(n1<n2){
        max=n2;
    }
    else{
        max=n1;
    }
    char **tab_digrammes1,**tab_digrammes2;
    tab_digrammes1=malloc(max*sizeof(char*)); //tab of all the digrammes in str1
    tab_digrammes2=malloc(max*sizeof(char*)); //tab of all the digrammes in str2
    char current_digramme1[3];   //current digramme at each iteration of the first loop
    char current_digramme2[3];   //current digramme at each iteration of the second loop
    int i=1,j=1;
    while((str1[i]!='\0'&&(str1[i]!='\n'))){
        tab_digrammes1[i-1]=malloc(3*sizeof(char));  //size of a digramme F
        current_digramme1[0]=str1[i-1];
        current_digramme1[1]=str1[i];
        current_digramme1[2]='\0';
        strcpy(tab_digrammes1[i-1],current_digramme1);
        //printf("%s ",tab_digrammes1[i-1]);
        //printf("i=%d\n",i);
        current_digramme1[0]='\0';
        i++;
    }
    //printf("\n");
    while(((str2[j]!='\0')&&(str2[j]!='\n'))){
        tab_digrammes2[j-1]=malloc(3*sizeof(char));  //size of a digramme F
        current_digramme2[0]=str2[j-1];
        current_digramme2[1]=str2[j];
        current_digramme2[2]='\0';
        strcpy(tab_digrammes2[j-1],current_digramme2);
        //printf("%s ",tab_digrammes2[j-1]);
        //printf("j=%d\n",j);
        current_digramme2[0]='\0';
        j++;
    }
    //printf("\n");
    //printf("%d %d",n1,n2);
    for(int z=0;z<(n1);z++){
        for(int k=0;k<(n2);k++){
            //printf("z=%d et k=%d\n",z,k);
            if(!strcmp(tab_digrammes1[z],tab_digrammes2[k])){
                count++;
                strcpy(tab_digrammes1[z],"$");
                strcpy(tab_digrammes2[k],"$");
                break;
            }
        }
    }
    int u=0;
    int v=0;
    while(u<i-1){
        free(tab_digrammes1[u++]);
        }
    while(v<j-1){
        free(tab_digrammes2[v++]);
    }
    free(tab_digrammes1);
    free(tab_digrammes2);
    return count;
}


/* (fonction de test)
float dice_distance_simple(char* str1,char* str2){
    float distance;
    int digramme_str1=number_of_digrammes(str1);
    int digramme_str2=number_of_digrammes(str2);
    if(!strcmp(str1,str2)){
        //printf("je rentre pour %s\t %s\n",str1,str2);
        //distance=0;
    }

    else if(((digramme_str1==0) || (digramme_str2==0))){
        //printf("je rentre NO DIGRAMS pour %s\t %s\n",str1,str2);
        distance=1;
    }
    else{
        distance=1-((2.0*compare_digrammes(str1,str2))/(digramme_str1+digramme_str2));
        //printf("je rentre dans le else");
    }
    return distance;
}
*/


void dice_distance(chained_list *list1,chained_list* list2,float* D,float* C){
    int n=list1->length;
    int m=list2->length;
    FILE* file=NULL;
    file=fopen("dice.pgm","w");
    fputs("P2\n",file);
    fprintf(file,"%d %d\n255\n",m,n);
    float distance=0;
    Cell* current_segment1=list1->first;
    Cell* current_segment2=list2->first;
    char* str1;
    char* str2;
    int digramme_str1;
    int digramme_str2;
    for(int i=0;i<n;i++){
        str1=current_segment1->str;
        for(int j=0;j<m;j++){
            str2=current_segment2->str;
            digramme_str1=number_of_digrammes(str1);
            digramme_str2=number_of_digrammes(str2);
            //printf("str1: %s \tstr2: %s\n",str1,str2);
            //printf("DIGRAMMES SAME: %d\n",compare_digrammes(str1,str2));
            //printf("DIGRAMME 1: %d\t DIGRAMME 2: %d\n",digramme_str1,digramme_str2);
            if(!strcmp(str1,str2)){
                distance=0;
            }
            else if(((digramme_str1==0) || (digramme_str2==0))){
                distance=1;
            }
            else{
                distance=1-((2.0*compare_digrammes(str1,str2))/(digramme_str1+digramme_str2));
            }
            //printf("%f\t",distance);
            fprintf(file,"%d ",(int)(255-(distance*255)));
            fflush(file);
            D[i*m+j]=distance;
            C[i*m+j]=distance;
            current_segment2=current_segment2->next;
        }
        fputc('\n',file);
        //printf("\n");
        current_segment2=list2->first;
        current_segment1=current_segment1->next;
    }
    fclose(file);
}


    // Levenshtein distance


int min(int a, int b, int c){
    if ((a < c)&&(a < b)){
        return a;
    }
    if((b < c)&&(b < a)){
        return b;
    }
    if ((c < a)&&(c < b)){
        return c;
    }
    return a;
}


int levenshtein_distance(char ch1[],char ch2[]){
    int l = strlen(ch1)+1; // lines
    int c = strlen(ch2)+1; // columns
    int tab[l*c] ; // matrix
    for(int g=0;g<l*c;g++){
        tab[g]=0;
    }
    for(int a=0;a<l;a++){
        tab[a*c]=a;
    }
    for(int b=0;b<c;b++){
        tab[b]=b;
    }
    /*for(int y=0; y<l*c; y++){
        printf("%d || ",tab[y]);
        if(((y+1)%c)==0){
            printf("\n");
        }
    }
    */
    int cost ; // initialisation of cost
    for(int i=1;i<l;i++){
        for(int j=1;j<c;j++){
            if (ch1[i-1]==ch2[j-1]){
                cost = 0 ;
            }
            else{
                cost = 1 ;
            }
        tab[(i*c)+j]=min(tab[((i-1)*c)+j]+1,tab[(i*c)+(j-1)]+1,tab[((i-1)*c)+(j-1)]+cost);
        }
    }
    return 1-(1/tab[(l*c)-1]);
}


void levenshtein_distance_matrix(chained_list *list1,chained_list* list2,float* D,float* C){
    int n=list1->length;
    int m=list2->length;
    FILE* file=NULL;
    file=fopen("levenshtein.pgm","w");
    fputs("P2\n",file);
    fprintf(file,"%d %d\n255\n",m,n);
    float distance=0;
    Cell* current_segment1=list1->first;
    Cell* current_segment2=list2->first;
    char* str1;
    char* str2;
    for(int i=0;i<n;i++){
        str1=current_segment1->str;
        for(int j=0;j<m;j++){
            str2=current_segment2->str;
            if(!strcmp(str1,str2)){
                distance=0;
            }
            else{
                distance=levenshtein_distance(str1,str2);
            }
            //printf("%f\t",distance);
            fprintf(file,"%d ",(int)(255-distance*255));
            fflush(file);
            D[i*m+j]=distance;
            C[i*m+j]=distance;
            current_segment2=current_segment2->next;
        }
        fputc('\n',file);
        //printf("\n");
        current_segment2=list2->first;
        current_segment1=current_segment1->next;
    }
    fclose(file);
}



// step 3 : coupling of segments, filtering and output


void fill_matrice_with_1(float* C,int n,int m){  //we need an initialised matrix filled with 1
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            C[i*m+j]=1;
        }
    }
}

void show_matrix(float* C,int n,int m){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            printf("%f\t",C[i*m+j]);
        }
        printf("\n");
    }
}

void minimal_coupling(chained_list* list1,chained_list* list2, float* C,float* D){
    int n=list1->length;   //lines
    int m=list2->length;   //columns
    int min_n_m;            //min(n,m)
    if(n<m){
        min_n_m=n;
    }
    else{
        min_n_m=m;
    }
    int min_i,min_j;
    float min;
    int w=0;
    int count=0;
    int* tab_min_i;
    int* tab_min_j;
    float* tab_min;
    tab_min=malloc(min_n_m*sizeof(float)+1);
    tab_min_i=malloc(min_n_m*sizeof(int));
    tab_min_j=malloc(min_n_m*sizeof(int));
    int k=0;
    while(count<min_n_m){ // while the tinier programme have a segment to associate
        min=2;
        for(int i=0; i<n; i++){
            for(int z=0;z<m;z++){
                if (C[i*m+z] < min){
                    min = C[i*m+z];    //we save the min in the matrix and its indexes
                    tab_min_i[k]=i;
                    tab_min_j[k]=z;
                    tab_min[k]=min;
                    //printf("%f",min);
                }
            }
        }
        min_i=tab_min_i[k];
        min_j=tab_min_j[k];
        //printf("n: %d m:%d",n,m);
        //printf("min : %f | line : %d | column: %d\n",tab_min[k],tab_min_i[k],tab_min_j[k]);
        for(int j=0; j<m; j++){
                C[min_i*m+j]=2; // we set all the elements of the line of the minimum at 2
        }
        for(int k=0; k<n; k++){
                C[k*m+min_j]=2; // we set all the elements of the column of the minimum at 2
        }
        //show_matrix(C,n,m);
        //printf("\n\n");
        count++;
        k++;
    }
    fill_matrice_with_1(C,n,m);
    while(w<k){
        C[tab_min_i[w]*m+tab_min_j[w]]=tab_min[w];
        //printf("index i: %d, index j: %d, min : %f",tab_min_i[w],tab_min_j[w],tab_min[w]);
        w++;
    }
    FILE* file=NULL;
    file=fopen("couplage.pgm","w");
    fputs("P2\n",file);
    fprintf(file,"%d %d\n255\n",m,n);
    for(int u=0;u<n;u++){
        for(int v=0;v<m;v++){
            fprintf(file,"%d ",(int)(255-(C[u*m+v]*255)));
            fflush(file);
        }
        fputc('\n',file);
    }
    //show_matrix(C,n,m);
    //printf("\n");
    fclose(file);
    free(tab_min);
    free(tab_min_i);
    free(tab_min_j);
}

void post_filtering(chained_list* list1,chained_list* list2, float* C,float* F){
    int n=list1->length;
    int m=list2->length;
    //printf("n: %d m: %d\n",n,m);
    FILE* file=NULL;
    file=fopen("filtrage.pgm","w");
    fputs("P2\n",file);
    fprintf(file,"%d %d\n255\n",m,n);
    int count=1,count_1=0,count_2=0;     //count: total number of elements in the sum, count_1: number of element in the top diag, count_2: number of elements in the bottom diag
    int k=-2;
    float sum=0;
    int u,v;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if((i>1)&&(j>1)&&(i<n-2)&&(j<m-2)){     //if we are inside enough of the matrix : 1<i<n-1 and 1<j<m-1
                while(k<3){
                    sum+=C[(i+k)*m+(j+k)];     //we sum the 4 adjacent coefficients located on the same diagonal
                    k++;
                }
                //printf("%f",sum);
                F[i*m+j]=sum/5;
                if(F[i*m+j]>0.7){           //if the coefficient is bigger than the treshold, it is worth 1
                    F[i*m+j]=1;
                    //printf("m");
                }
                //printf("%f\t",F[i*m+j]);
                sum=0;                             //we reset sum and k at their initial value
                k=-2;
            }
            else{
                u=i-1;
                v=j-1;
                //printf("Count %d\n",count);
                /*if((i==12)&(j==3)){
                    printf("u : %d v:%d\n",u,v);
                }*/
                sum+=C[i*m+j];
                while((u>=0) && (v>=0) && (count_1<2)){  //we sum at maximum  the 2 adjacent coefficients that are in the "top-left diagonal" and we count how many did we sum
                    sum+=C[u*m+v];
                    count_1++;    //we count how many time did we sum
                    u--;
                    v--;
                    /*if((i==12)&(j==3)){
                        printf("i enter in the top-left diag\n");
                    }*/
                }
                u=i+1;
                v=j+1;
                while((u<n)&&(v<m)&&(count_2<2)){ //we sum at maximum  the 2 adjacent coefficients that are in the "bottom-right diagonal"
                    sum+=C[u*m+v];
                    count_2++;         //we count how many time did we sum
                    u++;
                    v++;
                    /*if((i==12)&(j==3)){
                        printf("i enter in the bottom-right diag\n");
                    }*/
                }
                count+=count_1+count_2;     //total number of element added
                F[i*m+j]=sum/count;
                if((i==12)&&(j==3)){
                    //printf("Count : %d Sum %f\n",count,sum);
                    //printf("Count1: %d Count2: %d\n",count_1,count_2);
                }
                if(F[i*m+j]>0.7){   //if the coefficient is bigger than the treshold, it is worth 1
                    F[i*m+j]=1;
                    //printf("mm");
                }
                //printf("%f\t",F[i*m+j]);
                sum=0;           //we reset all our variables at their initial value
                count=1;
                count_1=0;
                count_2=0;
            }
            fprintf(file,"%d ",(int)(255-(F[i*m+j]*255)));
            fflush(file);
        }
        fputc('\n',file);
        //printf("\n");
    }
}

float result_output(float* F,int n,int m){   //function that returns the "distance" between the two files
    int min_n_m; //min(n,m)
    float delta;
    float sum=0;
    if(n>m){
        min_n_m=m;
    }
    else{
        min_n_m=n;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            sum+=(1-F[i*m+j]);
        }
    }
    delta=1-(sum/min_n_m);
    return delta;
}

int main(int argc,char *argv[]){
    chained_list* list1=malloc(sizeof(chained_list));   //list that stocks the lines of the first file
    chained_list* list2=malloc(sizeof(chained_list));    //list that stocks the lines of the 2nd file
    initialisation(list1);
    initialisation(list2);
    int number_of_lines;
    if(!opening_test(argv[1])){
        if(!opening_test(argv[2])){
            printf("The files \"%s\" and \"%s\" opened correctly!\n",argv[1],argv[2]);
            number_of_lines=count_lines(argv[1]);
            if(number_of_lines==-1){   //first arg: entry file // second arg: filtrated file // third arg: list with segments
                printf("The file \"%s\" is empty!\n",argv[1]);
                printf("---------------------------END---------------------------");
                return 0;
            }
            number_of_lines=count_lines(argv[2]);
            if (number_of_lines==-1){
                printf("The file \"%s\" is empty!\n",argv[2]);
                printf("---------------------------END---------------------------");
                return 0;
            }
            else{
                printf("Both files are not empty.\n");
                filtering(argv[1],"filtre1.c",list1);
                filtering(argv[2],"filtre2.c",list2);
            }
            //show(list1);
            //show(list2);
            float* D;
            float *C;
            float *F;
            C=malloc(list1->length*list2->length*sizeof(float));
            D=malloc(list1->length*list2->length*sizeof(float));
            F=malloc(list1->length*list2->length*sizeof(float));
            int answer ;
            printf("Choose the distance. 0 for Dice, 1 for Levenshtein.\n");
            scanf("%d",&answer);
            if (answer == 0){
                printf("---------------------------STARTING TO CALCULATE DICE DISTANCE--------------------------------------\n");
                dice_distance(list1,list2,D,C);
                printf("---------------------------DICE DISTANCE MATRIX HAS BEEN CREATED------------------------------------\n\n");
            }
            else if (answer == 1){
                printf("---------------------------STARTING TO CALCULATE LEVENSHTEIN DISTANCE-------------------------------\n");
                levenshtein_distance_matrix(list1,list2,D,C);
                printf("---------------------------LEVENSHTEIN DISTANCE MATRIX HAS BEEN CREATED-----------------------------\n");
            }
            else{
                printf("Error. You did not choose a correct answer.\n");
                return 0;
            }
            printf("---------------------------BEGINNING OF MINIMUM COUPLING BETWEEN SEGMENTS---------------------------\n");
            minimal_coupling(list1,list2,C,D);
            printf("---------------------------COUPLING MATRIX HAS BEEN CREATED-----------------------------------------\n\n");
            printf("---------------------------START OF POST FILTERING--------------------------------------------------\n");
            post_filtering(list1,list2,C,F);
            printf("---------------------------THE FILTRATED MATRIX HAS BEEN CREATED------------------------------------\n\n");
            printf("The final distance between the two files is %.2f.\n\n",result_output(F,list1->length,list2->length));
            free(D);
            free(C);
            free(F);
        }
        else{
            printf("The file \"%s\" does not exist and the program did not run.\n",argv[2]);
        }
    }
    else{
        printf("The file \"%s\" does not exist and the program did not run.\n",argv[1]);
    }
    destroy(list1);
    destroy(list2);
    printf("--------------------------------------------END-----------------------------------------------------\n");
    return 0;
}