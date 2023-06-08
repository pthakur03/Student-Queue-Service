#include "list.h"



/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked List. */
static User *create_user(char *name, UserType type, UserUnion data);
static int create_student(int num_classes, double *grades, Student *dataOut); //completed for you
static int create_instructor(double salary, Instructor *dataOut); //completed for you
static Node *create_node(char *name, UserType type, UserUnion data);
static int user_equal(const User *user1, const User *user2);

/** create_user
 *
 * 
 *
 * @param the fields of the User struct
 * @return a User, return NULL if malloc fails
 */
static User *create_user(char *name, UserType type, UserUnion data)
{
    struct user *createUser = (struct user *) malloc(sizeof(struct user));
    if (createUser == NULL) {
        return NULL;
    }
    if (name == NULL) {
        createUser->name = NULL;
    } else {
        createUser->name = (char *) malloc(strlen(name) + 1);
        if (createUser->name == NULL) {
            free(createUser);
            return NULL;
        }
        strcpy(createUser->name, name);
    }
    
    createUser->type = type;
    if (type == STUDENT) {
        if (create_student(data.student.num_classes, data.student.grades, &(createUser->data.student)) == 1) {
            free(createUser->name);
            free(createUser);
            return NULL;
        }
    } else if (type == INSTRUCTOR) {
        if (create_instructor(data.instructor.salary, &(createUser->data.instructor)) == 1) {
            free(createUser->name);
            free(createUser);
            return NULL;
        }
    }
    return createUser;
}

/** create_student
 *
 * 
 *
 * @param the fields of the Student struct, and an existing pointer to a student
 * @return 1 if malloc fails, 0 otherwise.
 */
static int create_student(int num_classes, double *grades, Student *dataOut)
{
    /***do not change anything in this function***/
    dataOut->num_classes = num_classes;
    dataOut->grades = NULL;
    if (grades != NULL) {
        if (!(dataOut->grades = (double *) malloc(sizeof(double)*num_classes))) return 1;
        memcpy(dataOut->grades, grades, sizeof(double)*num_classes);
    }
    return 0;
}

/** create_instructor
 *
 
 *
 * @param the fields of the Instructor struct, and an existing pointer to an instructor
 * @return 1 if malloc fails, 0 otherwise. (this function should always return 0 since
 * you won't need to malloc anything)
 */
static int create_instructor(double salary, Instructor *dataOut)
{
    /***do not edit anything in this function***/
    dataOut->salary = salary; //yes that's all this function does
    return 0;
}

/** create_node
  *
  * 
  *
  * @param the fields of the User struct
  * @return a Node
  */
static Node* create_node(char *name, UserType type, UserUnion data)
{
    struct node* newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL) {
        return NULL;
    }

    struct user* newUser = create_user(name, type, data);
    if (newUser == NULL) {
        free(newNode);
        return NULL;
    }
    
    newNode->data = newUser;
    newNode->next = NULL;
    return newNode;
}

/** student_equal
 *
 *
 *
 * @param the two Student structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int student_equal(const Student *student1, const Student *student2)
{
    if (student1->num_classes != student2->num_classes) return 0;
    if (student1->grades != student2->grades)
    {
        if (student1->grades == NULL || student2->grades == NULL) return 0;
        if (memcmp(student1->grades, student2->grades,
                student1->num_classes * sizeof(double)))
        {
            return 0;
        }
    }
    return 1;
}

/** user_equal
 * 
 *
 * @param the two User structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int user_equal(const User *user1, const User *user2)
{
    if (!user1 || !user2) {
        return 0;
    }
    
    if ((user1->name == NULL) ^ (user2->name == NULL)) {
        return 0;
    }

    if ((user1->name != NULL) && (user2->name != NULL)) {
        if (strcmp(user1->name, user2->name) != 0) {
            return 0;
        }
    }

    if (user1->type != user2->type) {
        return 0;
    }

    if (user1->type == STUDENT){
        return student_equal(&(user1->data.student), &(user2->data.student));
    } else {
        return user1->data.instructor.salary == user2->data.instructor.salary;
    }
    return 0;
}

/** create_list
 *
 * 
 *
 * @return a pointer to a new struct list or NULL on failure
 */
LinkedList *create_list(void)
{
    struct linked_list *createList = (struct linked_list *) malloc(sizeof(struct linked_list));

    if (createList == NULL) {
        return NULL;
    }

    createList->head = NULL;
    createList->size = 0;
    return createList;
}

/** push_front
 *
 * 
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_front(LinkedList *list, char *name, UserType type, UserUnion data)
{
    if (list == NULL) {
        return 1;
    }
    struct node* newNode = create_node(name, type, data);
    if (newNode == NULL) {
        return 1;
    }
    newNode->next = list->head;
    list->head = newNode;
    list->size += 1;
    return 0;
}

/** push_back
 *
 * 
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_back(LinkedList *list, char *name, UserType type, UserUnion data)
{
    if (list == NULL) {
        return 1;
    }
    struct node* newNode = create_node(name, type, data);
    if (newNode == NULL) {
        return 1;
    }
    newNode->next = NULL;
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        struct node* tempNode = list->head;
        while (tempNode->next != NULL) {
            tempNode = tempNode->next;
        }
        tempNode->next = newNode;
    }
    list->size += 1;
    return 0;
}

/** add_at_index
 *
 * 
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size]
 * @param the fields of the User struct
 * @return 1 if the index is out of bounds or the LinkedList is NULL or malloc fails
 *         (do not add the data in this case)
 *         otherwise (on success) return 0
 */
int add_at_index(LinkedList *list, int index, char *name, UserType type, UserUnion data)
{
    if (list == NULL) {
        return 1;
    }
    if (index < 0 || index > list->size) {
        return 1;
    }
    if (index == 0) {
        return push_front(list, name, type, data);
    } else if (index == list->size) {
        return push_back(list, name, type, data);
    } else {
        struct node* newNode = create_node(name, type, data);
        if (newNode == NULL) {
            return 1;
        }
        struct node* prevNode = list->head;
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->next;
        }
        newNode->next = prevNode->next;
        prevNode->next = newNode;
    }
    list->size += 1;
    return 0;
}

/** get
 *
 * 
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head.
 * @param dataOut A pointer to a pointer used to return the data from the
 *        specified index in the LinkedList or NULL on failure.
 * @return 1 if dataOut is NULL or index is out of range of the LinkedList or
 *         the LinkedList is NULL, 0 (on success) otherwise
 */
int get(LinkedList *list, int index, User **dataOut)
{
    if (list == NULL) {
        return 1;
    }
    if (index < 0 || index > list->size || !dataOut) {
        return 1;
    }
    struct node* currNode = list->head;
    int i = 0;
    while (i < index && currNode) {
        currNode = currNode->next;
        i++;
    }
    if (currNode == NULL) {
        return 1;
    }
    *dataOut = currNode->data;
    return 0;
}

/** contains
  *
  * 
  *
  *
  * @param list a pointer to the LinkedList structure
  * @param data The data, to see if it exists in the LinkedList
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the LinkedList or NULL on failure
  * @return int    0 if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else 1
  */
int contains(LinkedList *list, User *data, User **dataOut)
{
    if (dataOut == NULL) {
        return 0;
    }
    if (list == NULL || list->size == 0) {
        *dataOut = NULL;
        return 0;
    }
    struct node* currNode = list->head;
    for (int i = 0; i < list->size; i++) {
        if (user_equal(currNode->data, data) == 1) {
            *dataOut = currNode->data;
            return 1;
        }
        currNode = currNode->next;
    }
    *dataOut = NULL;
    return 0;
}

/** pop_front
  *
  * 
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_front(LinkedList *list, User **dataOut)
{
    if (list == NULL) {
        return 1;
    }
    if (list->head == NULL || dataOut == NULL) {
        return 1;
    }
    struct node* tempNode = list->head;
    *dataOut = tempNode->data;
    list->head = tempNode->next;
    free(tempNode);
    list->size -= 1;
    return 0;
}

/** pop_back
  *
  * 
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_back(LinkedList *list, User **dataOut)
{
    UNUSED(list);
    UNUSED(dataOut);
    if (list == NULL) {
        return 1;
    }
    if (list->head == NULL || dataOut == NULL) {
        return 1;
    }
    struct node* currNode = list->head;
    struct node* prevNode = NULL;
    while (currNode->next != NULL) {
        prevNode = currNode;
        currNode = currNode->next;
    }
    if (prevNode != NULL) {
        prevNode->next = NULL;
    } else {
        list->head = NULL;
    }
    *dataOut = currNode->data;
    free(currNode);
    list->size -= 1;
    return 0;
}


/** remove_at_index
 *
 * 
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to a pointer used to return the data in the last
 *                Node or NULL if the LinkedList is NULL or empty
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size-1]
 * @return 1 if the index is out of bounds, the LinkedList is NULL or
 *         the dataOut is NULL
 *         otherwise return 0
 */
int remove_at_index(LinkedList *list, User **dataOut, int index)
{
    if (list == NULL || dataOut == NULL) {
        return 1;
    }
    if (index < 0 || index >= list->size) {
        return 1;
    }
    if (list->size == 0){
        *dataOut = NULL;
        return 1;
    }
    if (index == 0) {
        return pop_front(list, dataOut);
    }

    if (index == list->size - 1) {
        return pop_back(list, dataOut);
    }
    struct node* currNode = list->head;
    struct node* prevNode = NULL;
    for (int i = 0; i < index; i++) {
        prevNode = currNode;
        currNode = currNode->next;
    }
    prevNode->next = currNode->next;
    *dataOut = currNode->data;
    free(currNode);
    list->size -= 1;
    return 0;
}

/** empty_list
 *
 * 
 *
 * @param list a pointer to the LinkedList structure
 */
void empty_list(LinkedList *list)
{
    if (list == NULL) {
        return;
    }
    struct node* currNode = list->head;
    while (currNode != NULL) {
        struct node* tempNode = currNode;
        currNode = currNode->next;
        if (tempNode->data->type == STUDENT) {
            free(tempNode->data->data.student.grades);
        }
        free(tempNode->data->name);
        free(tempNode->data);
        free(tempNode);
    }
    list->head = NULL;
    list->size = 0; 
}

/** num_passing_all_classes
 *
 *
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to int used to return the count of students passing all
 *                of their classes
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int num_passing_all_classes(LinkedList *list, int *dataOut)
{
    if (!list || list->size == 0 || !dataOut) {
        if (!list || list->size == 0) {
            *dataOut = -1;
        }
        return 1;
    }
    int count = 0;
    struct node* currNode = list->head;
    while (currNode != NULL) {
        if (currNode->data->type == STUDENT) {
            struct student* currStudent = &(currNode->data->data.student);
            int passingAll = 1;
            for (int i = 0 ; i < currStudent->num_classes; i++) {
                if (currStudent->grades[i] < 60) {
                    passingAll = 0;
                    break;
                }
            }
            if (passingAll) {
                count++;
            }
        }
        currNode = currNode->next;
    }
    *dataOut = count;
    return 0;
}

/** get_average_salary
 *
 * 
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to double used to return the average salary of the instructors
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int get_average_salary(LinkedList *list, double *dataOut)
{
    if (!list || list->size == 0 || !dataOut) {
        if (!list || list->size == 0) {
            *dataOut = -1;
        }
        return 1;
    }
    int count = 0;
    double salarySum = 0.0;
    struct node* currNode = list->head;
    while (currNode != NULL) {
        if(currNode->data->type == INSTRUCTOR) {
            struct instructor* currInstructor = &(currNode->data->data.instructor);
            salarySum += currInstructor->salary;
            count++;
        }
        currNode = currNode->next;
    }
    if (count == 0) {
        *dataOut = 0.0;
    } else {
        *dataOut = salarySum / count;
    }
    return 0;
}
