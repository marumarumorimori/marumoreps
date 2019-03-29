/* header files */
#include <stdio.h>
#include <stdlib.h>

/* macros */
#define N 8

/* functions */
int ArrayEnqueue(int *queue, int data, int *head, int *tail, size_t n);
int ArrayDequeue(int *queue, int *head, int *tail, size_t n);

/* main */
int main(void) {
    int queue[N];
    int head = 0, tail = 0;
    int data;

    /* 配列にエンキューする */
    ArrayEnqueue(queue, 10, &head, &tail, N);
    printf("Enqueue: %d\n", 10);

    ArrayEnqueue(queue, 20, &head, &tail, N);
    printf("Enqueue: %d\n", 20);

    ArrayEnqueue(queue, 30, &head, &tail, N);
    printf("Enqueue: %d\n", 30);

    ArrayEnqueue(queue, 40, &head, &tail, N);
    printf("Enqueue: %d\n", 40);

    /* 配列からデキューする */
    while ( tail - head ) {
        data = ArrayDequeue(queue, &head, &tail, N);
        printf("Dequeue: %d\n", data);
    }


    return EXIT_SUCCESS;
}

/**
 *  * 配列にエンキューする
 *   * @param[in,out] queue 配列
 *    * @param[in] data データ
 *     * @param[in,out] head キューの先頭要素
 *      * @param[in,out] tail キューの末尾要素
 *       * @param[in] n 配列の要素数
 *        * @retval >=1 配列に格納されている要素の数
 *         * @retval 0 配列へのエンキューに失敗
 *          */
int ArrayEnqueue(int *queue, int data, int *head, int *tail, size_t n) {
    if (*head % n != (*tail + 1) % n) {
        queue[(*tail)++ % n] = data;
        return *tail - *head;
    } else {
        return 0;
    }
}

/**
 *  * 配列からデキューする
 *   * @param[in,out] queue 配列
 *    * @param[in,out] head キューの先頭要素
 *     * @param[in,out] tail キューの末尾要素
 *      * @param[in] n 配列の要素数
 *       * @return デキューしたデータ
 *        */
int ArrayDequeue(int *queue, int *head, int *tail, size_t n) {
    if (*head != *tail) {
        return queue[(*head)++ % n];
    } else {
        return 0;
    }
}

