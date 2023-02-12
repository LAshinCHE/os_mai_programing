#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>     

pthread_mutex_t mutex;



typedef struct{
    long long point_1;
    long long point_2;
    int rounds;
    int sim;
} _args;

typedef struct
{
    int sim;
    int sum_1;
    int sum_2;
}_res;



void* roll_dice_model(void* args){
    _args* data = (_args *)args;
    _res* res = malloc(sizeof(_res));
    int rounds =  data->rounds;
    int player_dice_2;
    int player_dice_1;
    int num_simulations = data->sim;
    int sum_wins_1 = 0;
    int sum_wins_2 = 0;

    for (int i = 0; i < num_simulations; i++)
    {
        long long sum1 = data->point_1;
        long long sum2 = data->point_2;
        for (int i = 0; i < rounds; i++)
        {
            player_dice_1 =  rand() % 12 + 1;
            player_dice_2 =  rand() % 12 + 1;
            sum1 += player_dice_1;
            sum2 += player_dice_2;
        }
        if (sum1 > sum2)
            sum_wins_1 += 1;
        if (sum2 > sum1)
            sum_wins_2 += 1;

    }
    res->sum_1 = sum_wins_1;
    res->sum_2 = sum_wins_2;
    res->sim= num_simulations;
    return (void*) res;
}


int main(int  argc,char* argv[]){
    srand(time(NULL));
    if(argc == 1) {
        printf("Command line has no additional arguments\n");
        return 1;
    }
    int thread_count;
    if(sscanf(argv[1],"%d",&thread_count) == 0 || thread_count <= 0){
        printf("Argument fault\n");
        return 2;
    }
    printf("thread_count: %d \n", thread_count);
    int toure;
    double time_spent = 0.0;
    clock_t begin = clock();
    int total_sim;
    int total_sum1 = 0;
    int total_sum2 = 0;
    int k;
    _args *args = malloc(sizeof(_args));
    printf("Numbers of throw in total: \n");
    scanf("%d",&k);
    printf("Enter the toure number: \n");
    scanf("%d",&toure);
    printf("Enter the number of points the player 1 has: \n");
    scanf("%lld",&args->point_1);
    printf("Enter the number of points the player 2 has: \n");
    scanf("%lld",&args->point_2);
    printf("Number of experiments: \n");
    scanf("%d",&total_sim);
    args->sim = total_sim / thread_count;
    if (k < toure){
        perror("The number of tours and/or the tour number are entered incorrectly\n");
        return 3;
    }
    if(k == toure){
        if(args->point_1 > args->point_2)
            printf("The first player will win with a 100 percent probability\n");
        else if (args->point_1 < args->point_2)
            printf("The second player will win with a 100 percent probability\n");
        else
            printf("draw\n");
        return 0;
    }
    if (args->point_1 > k*12 || args->point_2 > k * 12 || args->point_1 < toure * 2 || args->point_2 < toure * 2){
        perror("The number of points the players have entered is incorrect\n");
        return 4;
    }

    args->rounds= k - toure;
    pthread_t th[thread_count];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < thread_count; i++){
        if (pthread_create(th + i, NULL,roll_dice_model, args) != 0){
            perror("Failed to create thread \n");
            return 4;
        }
    }
    for (int i = 0; i < thread_count; i++){
        _res* result;
        if (pthread_join(th[i],(void **) &result) != 0){
            perror("Failed phtread join \n");
            return 5;
        }
        printf("Num of simulations: %d \n", result->sim);
        printf("win first player rounds: %d \nwin second player rounts: %d\n \n",result->sum_1, result->sum_2);
        total_sum1 = total_sum1 + result->sum_1;
        total_sum2 = total_sum2 + result->sum_2;
        free(result);

    }
    pthread_mutex_destroy(&mutex);
    int probability_1 = (int)(((float)total_sum1 / (float)total_sim)*100);
    int probability_2 = (int)(((float)total_sum2 / (float)total_sim)*100);
    printf("win first player percent: %d%% \nwin second player percent: %d%%\n \n",probability_1, probability_2);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds", time_spent);
    free(args);
    return 0;
}
