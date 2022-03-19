#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_CREDITS 20

typedef struct students{//öğrenci bilgilerini tutan struct
    struct students *prev;
    int id;
    char name[20],surname[20];
    int total_nof_lessons;
    int total_credits;
    struct students *next;
}STUDENTS;

typedef struct lesson_reg{//ders kayıt işlemleri için tutulan struct
    int log_data;
    char lesson_name[10];
    int student_id;
    char date[10];
    char status[15];
    int mode;
    struct lesson_reg *next;
}LESSON_REG;

typedef struct lessons{//dersin kodu, adı, kapasitesi ve kredisi
    char lesson_code[10];
    char lesson_name[50];
    int lesson_capacity;
    int lesson_credits;
    int *reg_students;
    int attendancy;
    struct lessons *next;
}LESSONS;
int traverse_lesson_nodes(LESSONS **lessons_head,char name[10]);
int traverse_student_nodes(STUDENTS **students_head,int id);
void student_add_file(STUDENTS *p1,int mode);
void create_student_syllabbus(LESSON_REG *reg_head);
void add_student_information(STUDENTS *p1);
int add_node_student(STUDENTS **head,int mode,char buff[255],char delimiter[2]);
void delete_student_node(STUDENTS **head);
void update_student_file(STUDENTS *head);
void print_students(STUDENTS *head);
void lesson_add_file(LESSONS *p1,int mode);
int add_lesson_information(LESSONS *p1);
int find_lesson_order(char lesson_name[10],LESSONS *head);
int add_node_lesson(LESSONS **head,int mode,char buff[255],char delimiter[2]);
void update_lesson_reg_file(int log_data,char lesson_name[10], int std_id,int mode,char status[15]);
void delete_lesson_node(LESSONS **head,LESSON_REG *reg_head);
void sort_students_reg(LESSONS *p1);
void delete_lesson_file(LESSONS *head);
void print_lessons(LESSONS *head);
int add_node_reg(LESSON_REG **head,int log_data,char lesson_name[10],int student_id,char date[10],char status[15]);
LESSON_REG *traverse_node_reg(LESSON_REG *head,char lesson_name[10],int student_id);
void update_reg_file(LESSON_REG *head);
int update_lecture_reg(LESSONS *head_lessons,int std_id);
int find_last_log(LESSON_REG *head);
int add_student_to_lesson(LESSONS *head_lessons, STUDENTS *head_students,LESSON_REG **reg_head);
int remove_student_from_lesson(LESSONS *head_lessons, STUDENTS *head_students,LESSON_REG *reg_head);
void view_students_in_lesson(LESSONS *lessons_head);
void update_reg_status(LESSON_REG *head_reg,LESSONS *head_lessons);
void add_student(STUDENTS **head_students);
void add_lesson(LESSONS **head);
void delay_process();


int main() {
    setlocale(LC_ALL, "Turkish");
    STUDENTS *head_students=NULL;
    LESSONS *head_lessons=NULL;
    LESSON_REG *head_reg = NULL;

    char buff[255],delimiter[2];
    strcpy(delimiter,",\0");
    FILE *fp = fopen("students.txt", "r");
    while(fgets(buff, 255, fp) != NULL){
        add_node_student(&head_students,2,buff,delimiter);
    }
    fclose(fp);

    fp = fopen("lessons.txt", "r");
    while(fgets(buff, 255, fp) != NULL){
        add_node_lesson(&head_lessons,2,buff,delimiter);
    }
    fclose(fp);
    fp = fopen("lesson_reg.txt","r");
    LESSON_REG *std=(LESSON_REG*) malloc(sizeof (LESSON_REG));
    char *token;
    while(fgets(buff, 255, fp) != NULL) {
        token = strtok(buff, delimiter);
        std->log_data = atoi(token);

        token = strtok(NULL, delimiter);
        strcpy(std->lesson_name, token);

        token = strtok(NULL, delimiter);
        std->student_id = atoi(token);

        token = strtok(NULL, delimiter);
        strcpy(std->date, token);

        token = strtok(NULL, delimiter);
        token[strlen(token)-1]='\0';
        strcpy(std->status, token);
        add_node_reg(&head_reg,std->log_data,std->lesson_name,std->student_id,std->date,std->status);
    }
    fclose(fp);
    update_reg_status(head_reg,head_lessons);

    int menu_decision;
    do{
        printf("Please enter what do you want to do: \n");
        printf("1. Add a new student.\n");
        printf("2. Delete a student.\n");
        printf("3. Create a new lesson.\n");
        printf("4. Delete a lesson.\n");
        printf("5. Add student to a lesson\n");
        printf("6. Remove student from a lesson\n");
        printf("7. See the students attending to a lesson.\n");
        printf("8. See a student's syllabbus.\n");
        printf("9. View students.\n");
        printf("10. View lessons.\n");
        printf("11. Exit.\n");
        scanf("%d",&menu_decision);

        if(menu_decision==1){
            add_student(&head_students);
            delay_process();
        }
        else if(menu_decision==2){
            delete_student_node(&head_students);
            update_student_file(head_students);
            delay_process();
        }
        else if(menu_decision==3){
            add_lesson(&head_lessons);
            delay_process();
        }
        else if(menu_decision==4){
            delete_lesson_node(&head_lessons,head_reg);
            delete_lesson_file(head_lessons);
            delay_process();
        }
        else if(menu_decision==5){
            add_student_to_lesson(head_lessons,head_students,&head_reg);
            delay_process();
        }
        else if(menu_decision==6){
            remove_student_from_lesson(head_lessons,head_students,head_reg);
            delay_process();
        }
        else if(menu_decision==7) {
            view_students_in_lesson(head_lessons);
            delay_process();
        }
        else if(menu_decision==8){
            create_student_syllabbus(head_reg);
            delay_process();
        }
        else if(menu_decision==9){
            print_students(head_students);
            delay_process();
        }
        else if(menu_decision==10){
            print_lessons(head_lessons);
            delay_process();
        }
        else if(menu_decision!=11){
            printf("Undefined process,please enter your process again.\n");
        }
    }while(menu_decision!=11);


    return 0;
}
int traverse_lesson_nodes(LESSONS **lessons_head,char name[10]){
    //strcmp((*lessons_head)-
    while(*lessons_head != NULL && strcmp((*lessons_head)->lesson_code,name) != 0){
        *lessons_head = (*lessons_head) -> next;
    }
    if(*lessons_head == NULL){
        printf("There is no lesson with such id!");
        return -1;
    }
    else{
        return 0;
    }
}

int traverse_student_nodes(STUDENTS **students_head,int id){
    while(*students_head != NULL && (*students_head) -> id != id){
        *students_head = (*students_head) -> next;
    }

    if(*students_head == NULL){
        printf("There is no student with such id!");
        return -1;
    }
    else{
        return 0;
    }
}
void student_add_file(STUDENTS *p1,int mode){
    FILE *fp;
    if(mode==1){
        fp = fopen("students.txt","a");
    }
    if(mode==2){
        fp = fopen("students.txt","w");
    }
    fprintf(fp,"%d,%s,%s,%d,%d\n", p1->id,p1->name,p1->surname, p1->total_credits,p1->total_nof_lessons);
    fclose(fp);
}

void create_student_syllabbus(LESSON_REG *reg_head){
    int std_id,i;
    char string_std_id[15];
    if(reg_head == NULL){
        printf("There is no lesson register in the system!\n");
    }
    else{
        printf("To see a student's syllabbus and crate a file\n");
        printf("Please enter id of the student: \n");
        scanf("%d",&std_id);
        itoa(std_id,string_std_id,10);
        strcat(string_std_id,".txt");
        FILE *fp = fopen( string_std_id,"w");
        printf("Student Sylabbus:\n");
        i=0;
        while(reg_head->next!=NULL){
            if(std_id == reg_head->student_id && strcmp(reg_head->status,"registered") == 0){
                printf("%s\n",reg_head->lesson_name);
                fprintf(fp,"%s\n",reg_head->lesson_name);
                i++;
            }
            reg_head=reg_head->next;
        }
        if(i==0){
            printf("This student is not registered to any lesson yet.\n");
        }
        fclose(fp);
    }

}

void add_student_information(STUDENTS *p1){
    printf("Name and surname of the student?  ");
    scanf("%s %s",p1->name,p1->surname);
    printf("Student id?  ");
    scanf("%d",&p1->id);
    p1->total_nof_lessons = 0;
    p1->total_credits = 0;
    student_add_file(p1,1);
}
int find_student_order(int std_id,STUDENTS *head){
    int i=0;
    while(head!=NULL && std_id > head->id) {
        head = head->next;
        i++;
    }
    //printf("control -----------> %d\n", i );
    if(i==0){
        return 1;
    }
    else if(head == NULL){
        return 2;
    }
    else{
        return 3;
    }
}

int add_node_student(STUDENTS **head,int mode,char buff[255],char delimiter[2]){
    STUDENTS *std;
    std = (STUDENTS*) malloc(sizeof(STUDENTS) );
    if(mode==1){
        add_student_information(std);
    }
    else{
        char *token;
        token = strtok(buff, delimiter);
        std->id= atoi(token);

        token = strtok(NULL, delimiter);
        strcpy(std->name, token);

        token = strtok(NULL, delimiter);
        strcpy(std->surname, token);

        token = strtok(NULL, delimiter);
        std->total_credits= atoi(token);

        token = strtok(NULL, delimiter);
        std->total_nof_lessons= atoi(token);
    }
    if(*head!=NULL){
        STUDENTS *tmp2=*head;
        while(tmp2 -> next != NULL && std->id != tmp2->id){
           // printf("1\n");
            tmp2 = tmp2->next;
        }
        if(std->id == tmp2->id){
            printf("Student with that ID already exists!\n");
            return -1;
        }
    }
    int status = find_student_order(std->id,*head);
    if(status==1){//adding node to start
        std->next=*head;
        *head=std;
        std->prev = *head;
    }
    else if(status==2){//adding node to end
        STUDENTS *tmp ;
        tmp = *head;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        std->prev = tmp;
        tmp->next = std;
        std->next = NULL;
    }
    else{//adding node to middle
        STUDENTS *tmp=*head;
        while(tmp->next!=NULL && tmp->next->id < std->id ){
            tmp = tmp->next;
        }
        std->prev = tmp;
        std->next = tmp->next;
        tmp->next = std;
    }
    return 0;
}
void delete_student_node(STUDENTS **head){
    int std_id,status;
    STUDENTS *tmp,*tmp2;
    printf("Please enter the id of the student you would like to delete from the system: \n");
    scanf("%d",&std_id);

    status = find_student_order(std_id,*head);
    if(status==1){
        if(*head==NULL){
            printf("Warning: Empty Student List!\n");
        }
        else if((*head)->id == std_id){
            tmp=(*head)->next;
            printf("Student with id : %d has been deleted.\n",std_id);
            free(*head);
            *head = tmp;
        }
        else{
            printf("There is no student with such ID!\n");
        }
    }
    else if(status==2){
        if(*head==NULL){
            printf("Warning: Empty Student List\n");
        }
        else{
            if((*head)->next==NULL){
                free((*head)->next);
                head =NULL;
                printf("Student with id : %d has been deleted.\n",std_id);
            }
            else{
                tmp = *head;
                while(tmp->next->next!=NULL){
                    tmp = tmp->next;
                }
                if((*head)->id == std_id){
                    free(tmp->next->next);
                    tmp->next=NULL;
                    printf("Student with id : %d has been deleted.\n",std_id);
                }
                else{
                    printf("There is no student with such ID!\n");
                }
            }
        }
    }
    else{
        if((*head)->id==std_id){
            tmp=(*head)->next;
            free(*head);
            printf("Student with id : %d has been deleted.\n",std_id);
            (*head)->next=NULL;
            *head = tmp;
        }
        else{
            tmp = *head;
            while(tmp->next!=NULL && tmp->next->id != std_id ){
                tmp = tmp->next;
            }
            if(tmp->next== NULL){
                printf("There is no student with such ID!\n");
            }
            else{
                tmp2=tmp->next;
                tmp->next=tmp->next->next;
                printf("Student with id : %d has been deleted.\n",std_id);
                free(tmp2);
                tmp2=NULL;
            }
        }
    }

}
void update_student_file(STUDENTS *head){
    student_add_file(head,2);
    while(head->next!=NULL){
        head = head->next;
        student_add_file(head,1);
    }
}

void print_students(STUDENTS *head){
    while(head!=NULL){
        printf("\n------------------------------------------------\n");
        printf("Student: %s %s \n", head->name,head->surname);
        printf("Student id: %d\n", head->id);
        printf("Number of lesson that student has: %d\n", head->total_nof_lessons);
        printf("Total credits that student has: %d\n",head->total_credits);
        printf("\n------------------------------------------------\n");
        head = head->next;
    }
}

void lesson_add_file(LESSONS *p1,int mode){
    FILE *fp;
    if(mode==1){
        fp = fopen("lessons.txt","a");
    }
    if(mode==2){
        fp = fopen("lessons.txt","w");
    }
    fprintf(fp,"%s,%s,%d,%d\n", p1->lesson_code,p1->lesson_name,p1->lesson_capacity,p1->lesson_credits);
    fclose(fp);
}
int add_lesson_information(LESSONS *p1){
    printf("Code of the lesson?  \n");
    scanf("%s", p1->lesson_code);
    printf("Name of the lesson?  \n");
    scanf("%s", p1->lesson_name);
    printf("What is the capacity of this lesson?\n");
    scanf("%d",&p1->lesson_capacity);
    printf("How many credits is the lesson?\n");
    scanf("%d",&p1->lesson_credits);
    //printf("Lesson teacher?\n");
    //scanf("%s",p1->lesson_teacher);
    p1->attendancy = 0;
    lesson_add_file(p1,1);
    return 0;
}

int find_lesson_order(char lesson_name[10],LESSONS *head){
    int i=0;
    //printf("%d",strcmp(lesson_name,head->lesson_name));
    while(head!=NULL &&  strcmp(lesson_name,head->lesson_code) > 0) {
        head = head->next;
        i++;
    }
    //printf("control -----------> %d\n", i );
    if(i==0){
        return 1;
    }
    else if(head == NULL){
        return 2;
    }
    else{
        return 3;
    }
}

int add_node_lesson(LESSONS **head,int mode,char buff[255],char delimiter[2]){
    LESSONS *std;
    std = (LESSONS *) malloc(sizeof(LESSONS) );

    if(mode==1){
        add_lesson_information(std);
    }
    else{
       // printf("ctrl2");
        char *token;
        token = strtok(buff, delimiter);
        strcpy(std->lesson_code, token);

        token = strtok(NULL, delimiter);
        strcpy(std->lesson_name, token);

        token = strtok(NULL, delimiter);
        std->lesson_credits = atoi(token);

        token = strtok(NULL, delimiter);
        std->lesson_capacity= atoi(token);
        std->attendancy = 0;
    }
    if(*head!=NULL){
        LESSONS *tmp2=*head;
        //strcmp(std->lesson_name,tmp2->lesson_name)
        while(tmp2 -> next != NULL && strcmp(std->lesson_code,tmp2->lesson_code) != 0){
            // printf("1\n");
            tmp2 = tmp2->next;
        }
        if(strcmp(std->lesson_code,tmp2->lesson_code) == 0){
            printf("Lesson with that ID already exists!\n");
            return -1;
        }
    }

    int status = find_lesson_order(std->lesson_code,*head);
    if(status==1){//adding node to start
        std->next=*head;
        *head=std;
    }
    else if(status==2){//adding node to end
        LESSONS *tmp ;
        tmp = *head;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        tmp->next = std;
        std->next = NULL;
    }
    else{//adding node to middle
        LESSONS *tmp=*head;
        //strcmp(tmp->next->lesson_name,std->lesson_name)
        while(tmp->next!=NULL && strcmp(tmp->next->lesson_code,std->lesson_code)<0 ){
            tmp = tmp->next;
        }
        std->next = tmp->next;
        tmp->next = std;
    }
    //std->reg_students = (int*)calloc(1,sizeof(int));
    std->reg_students = (int*)malloc(1*sizeof(int));
    return 0;
}
void update_lesson_reg_file(int log_data,char lesson_name[10], int std_id,int mode,char status[15]){
    FILE *fp ; //fopen("lesson_reg.txt","a");
    if(mode==1){
        fp = fopen("lesson_reg.txt","a");
        fprintf(fp,"%d,%s,%d,%s,%s\n",log_data,lesson_name, std_id,"date",status);
    }
    if(mode==2){
        fp = fopen("lesson_reg.txt","w");
        fprintf(fp,"%d,%s,%d,%s,%s\n",log_data,lesson_name, std_id,"date",status);
    }
  fclose(fp)  ;
}
void delete_lesson_node(LESSONS **head,LESSON_REG *reg_head){
    LESSON_REG *tmp_reg = reg_head;
    int status,flag=0,attendancy = 0;
    char lesson_name[10];
    LESSONS *tmp,*tmp2;
    printf("Please enter the id of the lesson you would like to delete from the system: \n");
    scanf("%s",lesson_name);

    status = find_lesson_order(lesson_name,*head);
    if(status==1){
        if(*head==NULL){
            printf("lesson: Empty lesson List!\n");
        }
        //strcmp((*head)->lesson_name,lesson_name)
        else if(strcmp((*head)->lesson_code,lesson_name)==0){
            attendancy = (*head)->attendancy;
            tmp=(*head)->next;
            printf("lesson with id : %s has been deleted.\n",lesson_name);
            flag=1;
            free(*head);
            *head = tmp;
        }
        else{
            printf("There is no lesson with such ID!\n");
        }
    }
    else if(status==2){
        if(*head==NULL){
            printf("Warning: Empty lesson List\n");
        }
        else{
            if((*head)->next==NULL){
                attendancy = (*head)->attendancy;
                free((*head)->next);
                head =NULL;
                printf("lesson with id : %s has been deleted.\n",lesson_name);
                flag=1;
            }
            else{
                tmp = *head;
                while(tmp->next->next!=NULL){
                    tmp = tmp->next;
                }
                //strcmp((*head)->lesson_name,lesson_name)
                if(strcmp((*head)->lesson_code,lesson_name)==0){
                    attendancy = tmp->next->next->attendancy;
                    free(tmp->next->next);
                    tmp->next=NULL;
                    printf("lesson with id : %s has been deleted.\n",lesson_name);
                    flag=1;
                }
                else{
                    printf("There is no lesson with such ID!\n");
                }
            }
        }
    }
    else{
        if(strcmp((*head)->lesson_code,lesson_name)==0){
            attendancy = (*head)->attendancy;
            tmp=(*head)->next;
            free(*head);
            printf("Lesson with id : %s has been deleted.\n",lesson_name);
            flag = 1;
            (*head)->next=NULL;
            *head = tmp;
        }
        else{
            tmp = *head;
            //strcmp(tmp->next->lesson_name,lesson_name)
            while(tmp->next!=NULL && strcmp(tmp->next->lesson_code,lesson_name) !=0 ){
                tmp = tmp->next;
            }
            if(tmp->next== NULL){
                printf("There is no lesson with such ID!\n");
            }
            else{
                attendancy = tmp->attendancy;
                tmp2=tmp->next;
                tmp->next=tmp->next->next;
                printf("Lesson with id : %s has been deleted.\n",lesson_name);
                flag = 1;
                free(tmp2);
                tmp2=NULL;
            }
        }
    }
    if(flag==1){
        while( tmp_reg!=NULL){
            if(strcmp(lesson_name,tmp_reg->lesson_name) ==0 ){
                strcpy(tmp_reg->status,"lesson_deleted");
            }
            tmp_reg=tmp_reg->next;
        }
        update_reg_file(reg_head);
    }
}
void sort_students_reg(LESSONS *p1){
    int i, key, j;
    for (i = 1; i < p1->attendancy ; i++) {
        key = p1->reg_students[i];
        j = i - 1;

        while (j >= 0 && p1->reg_students[j] > key) {
            p1->reg_students[j + 1] = p1->reg_students[j];
            j = j - 1;
        }
        p1->reg_students[j + 1] = key;
    }
}

void delete_lesson_file(LESSONS *head){
    lesson_add_file(head,2);
    while(head->next!=NULL){
        head = head->next;
        lesson_add_file(head,1);
    }
}

void print_lessons(LESSONS *head){
    while(head!=NULL){
        printf("\n------------------------------------------------\n");
        printf("Lesson id: %s\n",head->lesson_code);
        printf("Lesson name: %s\n",head->lesson_name);
        printf("Credits of the lesson %d\n", head->lesson_credits);
        printf("Capacity of the lesson: %d\n",head->lesson_capacity);
        printf("\n------------------------------------------------\n");
        head = head->next;
    }
}

int add_node_reg(LESSON_REG **head,int log_data,char lesson_name[10],int student_id,char date[10],char status[15]){
    LESSON_REG *std;
    std = (LESSON_REG *) malloc(sizeof(LESSON_REG) );
    std->next = NULL;

    std->log_data = log_data;
    strcpy(std->lesson_name,lesson_name);
    std->student_id = student_id;
    strcpy(std->date, date);
    strcpy(std->status, status);

    LESSON_REG *tmp;
    if(*head==NULL){
        *head = std;
    }else{
        tmp = *head;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        tmp->next = std;
    }
    return 0;
}

LESSON_REG *traverse_node_reg(LESSON_REG *head,char lesson_name[10],int student_id){
    //strcmp(lesson_name,head->lesson_name)
    while(strcmp(lesson_name,head->lesson_name) !=0 || student_id != head->student_id ){
        head=head->next;
    }
    return head;
}

void update_reg_file(LESSON_REG *head){
    head->mode = 2;
    while(head ->next  != NULL){
        update_lesson_reg_file(head->log_data,head->lesson_name,head->student_id,head->mode,head->status);
        head = head->next;
        head->mode = 1;
    }
    head->mode = 1;
    update_lesson_reg_file(head->log_data,head->lesson_name,head->student_id,head->mode,head->status);
}
int update_lecture_reg(LESSONS *head_lessons,int std_id){
    head_lessons->reg_students = (int *) realloc(head_lessons->reg_students, (head_lessons->attendancy+ 1) * sizeof(int));
    head_lessons->reg_students[head_lessons->attendancy] = std_id;
}
int find_last_log(LESSON_REG *head){
    int log_data;
    while(head ->next  != NULL){
        head = head->next;
    }
    log_data = head->log_data;
    return log_data;
}
int add_student_to_lesson(LESSONS *head_lessons, STUDENTS *head_students,LESSON_REG **reg_head){
    int log_data,i;
    int control=0,student_id;
    char lesson_name[10];
    STUDENTS *tmp = head_students;
    LESSONS *tmp_lesson = head_lessons;
    printf("Please enter lesson name:");
    scanf("%s",lesson_name);
    control = traverse_lesson_nodes(&head_lessons,lesson_name);
    if(control == -1){
        return -1;
    }
    if(head_lessons->attendancy== head_lessons->lesson_capacity){
        printf("Max capacity at this lesson has reached!\n");
        return -1;
    }
    control = 0;
    do{
        printf("ID of the student you would like to add: ");
        scanf("%d",&student_id);
        i=0;
        while (i < head_lessons->attendancy && student_id!=head_lessons->reg_students[i]){
            i++;
        }
        if(i==head_lessons->attendancy){
            control = traverse_student_nodes(&tmp,student_id);
            if(control!=-1) {
                if(tmp->total_credits + head_lessons->lesson_credits >= MAX_CREDITS){
                    printf("This student has reached to maximum credits!\n");
                }
                else{
                    log_data = find_last_log(*reg_head);
                    log_data++;
                    update_lecture_reg(head_lessons,student_id);
                    add_node_reg(&*reg_head,log_data,lesson_name,student_id,"date","registered");
                    head_lessons -> attendancy ++;
                    tmp->total_credits += head_lessons->lesson_credits;
                    tmp->total_nof_lessons ++;
                }

            }
        }
        else{
            printf("This student was already added to the this lesson!\n");
        }
        if(head_lessons->attendancy <= head_lessons->lesson_capacity){
            printf("If you want to add another student to the lesson, please press 1\n");
            scanf("%d", &control);
        }
        else{
            printf("Max capacity at this lesson has reached!\n");
            control = 0;
        }
        tmp = head_students;
    }while(control == 1);
    sort_students_reg(head_lessons);
    update_reg_file(*reg_head);
    update_student_file(head_students);
    return 0;
}


int remove_student_from_lesson(LESSONS *head_lessons, STUDENTS *head_students,LESSON_REG *reg_head){
    int student_id,control = 0;
    char name[10];
    STUDENTS *tmp ;
    LESSONS *tmp_lesson = head_lessons;
    printf("Please enter lesson id:");
    scanf("%s",name);
    control = traverse_lesson_nodes(&tmp_lesson,name);
    if(control==-1){
        return -1;
    }
    int i,j;
    do{
        tmp = head_students;
        printf("ID of the student you would like to remove: ");
        scanf("%d",&student_id);
        while(tmp != NULL && tmp -> id != student_id){
            tmp= tmp -> next;
        }
        if(tmp!=NULL) {
            i = 0;
            while (i < head_lessons->attendancy && student_id != head_lessons->reg_students[i]) {
                i++;
            }
            if (i+1 == head_lessons->attendancy) {
                LESSON_REG *tmp_reg = NULL;
                tmp_reg = traverse_node_reg(reg_head, name, student_id);
                strcpy(tmp_reg->status, "unregistered");
                j = 0;
                while (tmp_lesson->reg_students[j] != student_id) {
                    j++;
                }
                for (; j < tmp_lesson->attendancy; j++) {
                    tmp_lesson->reg_students[j] = tmp_lesson->reg_students[j + 1];
                }
                tmp_lesson->attendancy--;
                tmp->total_nof_lessons--;
                tmp->total_credits -= tmp_lesson->lesson_credits;
            }
            else {
                printf("This student is already not in this lesson!\n");
            }
        }
        else{
            printf("There is no student with such id!\n");
        }
        printf("If you want to remove another student to the lesson, please press 1\n");
        scanf("%d", &control);
    }while(control == 1);
    update_reg_file(reg_head);
    return 0;
}

void view_students_in_lesson(LESSONS *lessons_head){
    int i=0;
    char name[10];
    printf("Which lesson?");
    scanf("%s",name);
    int control = traverse_lesson_nodes(&lessons_head,name);
    if(control!=-1){
        printf("Students to registered to the lesson with id: %s, name: %s are:\n", lessons_head->lesson_code,lessons_head->lesson_name);
        for(i=0;i<lessons_head->attendancy;i++){
            printf("%d\n",lessons_head->reg_students[i]);
        }
    }

}

void update_reg_status(LESSON_REG *head_reg,LESSONS *head_lessons){
    LESSONS *tmp;
    while(head_reg != NULL ){
        tmp = head_lessons;
        while(tmp != NULL ){
            if(strcmp(tmp->lesson_code, head_reg->lesson_name) == 0){
                update_lecture_reg(tmp,head_reg->student_id);
                tmp->attendancy++;
            }
            tmp = tmp->next;
        }
        head_reg= head_reg -> next;
    }

}

void add_student(STUDENTS **head_students){
    char buff[255],delimiter[2];
    int control;
    do{
        printf("Please complete the information of the student\n");
        add_node_student(&*head_students,1,buff,delimiter);
        printf("if you want to add another student please press 1\n");
        scanf("%d",&control);
    }while(control==1);
    printf("Do you wish to see students that has been added? \n For yes please type 1\n");
    scanf("%d",&control);
    if(control==1){
        printf("Student information that you entered:\n");
        print_students(*head_students);
    }
}

void add_lesson(LESSONS **head){
    char buff[255],delimiter[2];
    int control;
    do{
        printf("please complete the information of the lesson\n");
        add_node_lesson(&*head,1,buff,delimiter);
        printf("if you want to add another lesson please press 1\n");
        scanf("%d",&control);
    }while(control==1);
    printf("Do you wish to see the lessons that has been added? \n For yes please type 1\n");
    scanf("%d",&control);
    if(control==1){
        printf("Lesson information that you entered:\n");
        print_lessons(*head);
    }

}

void delay_process(){
    int delay;
    printf("Press any key to progress.\n");
    scanf("%d",&delay);
}


