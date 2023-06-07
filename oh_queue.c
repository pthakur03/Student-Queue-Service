// DO NOT MODIFY THE INCLUDE(S) LIST
#include <stdio.h>
#include "oh_queue.h"

struct Queue oh_queue;

/** push
 * @brief Create a new student and push him
 * onto the OH queue
 * @param studentName pointer to the student's name
 * @param topicName topic the student has a question on
 * @param questionNumber hw question number student has a question on
 * @param pub_key public key used for calculating the hash for customID
 * @return FAILURE if the queue is already at max length, SUCCESS otherwise
 */
int push(const char *studentName, const enum subject topicName, const float questionNumber, struct public_key pub_key){
    if (studentName == NULL) {
        return FAILURE;
    }
    if (oh_queue.stats.no_of_people_in_queue + 1 > MAX_QUEUE_LENGTH) {
        return FAILURE;
    }
    struct Student newStudent;
    struct StudentData newStudentData;
    struct Topic newStudentTopic;

    newStudentTopic.questionNumber = questionNumber;
    newStudentTopic.topicName = topicName;
    newStudentData.topic = newStudentTopic;

    if (my_strlen(studentName) >= MAX_NAME_LENGTH) {
        my_strncpy(newStudentData.name, studentName, MAX_NAME_LENGTH - 1);
        const char *endblock = "\0";
        my_strncat(newStudentData.name, endblock, 1);
    } else {
        my_strncpy(newStudentData.name, studentName, my_strlen(studentName) + 1);
    }
    int cipher[MAX_NAME_LENGTH];
    hash(cipher, newStudentData.name, pub_key);
    for (int i = 0; i < (int)my_strlen(newStudentData.name) + 1; i++) {
        newStudent.customID[i] = cipher[i];
    }

    newStudent.queue_number = oh_queue.stats.no_of_people_in_queue + oh_queue.stats.no_of_people_visited;
    newStudent.studentData = newStudentData;
    oh_queue.students[oh_queue.stats.no_of_people_in_queue] = newStudent;
    oh_queue.stats.no_of_people_in_queue++;

    OfficeHoursStatus(&oh_queue.stats);
    return SUCCESS;
}

/** pop
 * @brief Pop a student out the OH queue
 * @return FAILURE if the queue is already at empty, SUCCESS otherwise
 */
int pop(void) {
    if (oh_queue.stats.no_of_people_in_queue == 0) {
        return FAILURE;
    } 
    for (int i = 0; i < oh_queue.stats.no_of_people_in_queue - 1; i++) {
        oh_queue.students[i] = oh_queue.students[i + 1];
    }
    oh_queue.stats.no_of_people_in_queue--;
    oh_queue.stats.no_of_people_visited++;
    OfficeHoursStatus(&oh_queue.stats);
    return SUCCESS;
}

/** group_by_topic
 * @brief Push pointers to students, who match the given topic, to
 * the given array "grouped"
 * @param topic the topic the students need to match
 * @param grouped an array of pointers to students
 * @return the number of students matched
 */
int group_by_topic(struct Topic topic, struct Student *grouped[]) { 
    int counter = 0;
    for (int i = 0; i < (oh_queue.stats.no_of_people_in_queue); i++) {
        if ((oh_queue.students[i].studentData.topic.questionNumber == topic.questionNumber) && (oh_queue.students[i].studentData.topic.topicName == topic.topicName)) {
            *grouped = &oh_queue.students[i];
            grouped++;
            counter++;
        }
    }
    return counter;
}

/** hash
 * @brief Creates a hash based on pub_key provided
 * @param ciphertext the pointer where you will store the hashed text
 * @param plaintext the originak text you need to hash
 * @param pub_key public key used for calculating the hash
 */
void hash(int *ciphertext, char *plaintext, struct public_key pub_key) {
    size_t plainTextLen = my_strlen(plaintext);
    for (int i = 0; i < (int)plainTextLen; i++) {
        ciphertext[i] = power_and_mod((int)plaintext[i], pub_key.e, pub_key.n);
    }
    return;
}

/** update_student
 * @brief Find the student with the given ID and update his topic
 * @param customID a pointer to the id of the student you are trying to find
 * @param newTopic the new topic that should be assigned to him
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int update_student(struct Topic newTopic, int *customID) {
    for (int i = 0; i < oh_queue.stats.no_of_people_in_queue; i++) {
        int studentNameLen = my_strlen(oh_queue.students[i].studentData.name);
        for (int j = 0; j < studentNameLen; j++) {
            if (oh_queue.students[i].customID[j] != customID[j]) {
                break;
            }
            if (j == studentNameLen - 1) {
                oh_queue.students[i].studentData.topic = newTopic;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/** remove_student_by_name
 * @brief Removes first instance of a student with the given name
 * @param name the name you are searching for
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int remove_student_by_name(char *name){
    int found = 0;
    for (int i = 0; i < oh_queue.stats.no_of_people_in_queue; i++) {
        if (my_strncmp(oh_queue.students[i].studentData.name, name, my_strlen(oh_queue.students[i].studentData.name)) == 0) {
            found = 1;
            for (int j = i; j < oh_queue.stats.no_of_people_in_queue - 1; j++) {
                oh_queue.students[j] = oh_queue.students[j + 1];
            }
            oh_queue.stats.no_of_people_in_queue--;
            oh_queue.stats.no_of_people_visited++;
            OfficeHoursStatus(&oh_queue.stats);
            break;
        }
    }
    if (!found) {
        return FAILURE;
    }
    return SUCCESS;
}

/** remove_student_by_topic
 * @brief Remove all instances of students with the given topic
 * @param topic the topic you are trying to remove from the queue
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int remove_student_by_topic(struct Topic topic) {
    int found = 0;
    if (oh_queue.stats.no_of_people_in_queue == 0) {
        return FAILURE;
    }
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        if (oh_queue.students[i].studentData.topic.topicName == topic.topicName
            && oh_queue.students[i].studentData.topic.questionNumber == topic.questionNumber) {
            for (int j = i; j < MAX_QUEUE_LENGTH - 1; j++) {
                oh_queue.students[j] = oh_queue.students[j+1];
            }
            i--;
            oh_queue.students[MAX_QUEUE_LENGTH - 1].customID[0] = 0;
            oh_queue.stats.no_of_people_visited++;
            oh_queue.stats.no_of_people_in_queue--;
            found = 1;
            OfficeHoursStatus(&oh_queue.stats);
        }
    }
    if (!found) {
        return FAILURE;
    }
    return SUCCESS;
}

/** OfficeHoursStatus
 * @brief Updates the "currentStatus" field based on
 * whether or not all students in the queue have been helped
 * @param resultStats A pointer the OfficeHoursStats variable
 * you are to update
 */
void OfficeHoursStatus(struct OfficeHoursStats* resultStats ){
    if (resultStats->no_of_people_in_queue == 0) {
        oh_queue.stats.currentStatus = "Completed";
    } else {
        oh_queue.stats.currentStatus = "InProgress";
    }
    return;
}

/*
 * Calculates (b^e)%n without overflow
 */
int power_and_mod(int b, int e, int n) {
    long int currNum = 1;
    for (int i = 0; i < e; i++) {
        currNum *= b;
        if (currNum >= n) {
            currNum %= n;
        }
    }
    return (int) (currNum % n);
}
