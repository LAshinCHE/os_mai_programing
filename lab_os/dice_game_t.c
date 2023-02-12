#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

pthread_mutex_t mutex;


typedef struct{
    long long summ1;
    long long summ2;
    int rounds;
    int sim;
} _args;

typedef struct 
{
    int firs_p;
    int second_p;
}_res;


void* roll_dice_model(void* args){
    _args* data = (_args *)args;
    long long sum1 = data->summ1; 
    long long sum2 = data->summ2;
    int rounds =  data->rounds;
    int player_dice_2;
    int player_dice_1;
    int num_simulations = data->sim;
    int sum_wins_1 = 0;
    int sum_wins_2 = 0;
    for (long long i = 0; i < num_simulations; i++)
    {
        sum1 = 0;
        sum2 = 0;
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
    // loock at float numbers
    int percentages_1 = (int)((float)sum_wins_1 / (float)num_simulations * 100);
    int percentages_2 = (int)((float)sum_wins_2 / (float)num_simulations * 100);
    free(args);
    _res* res = malloc(sizeof(_res));
    res->firs_p = percentages_1;
    res->second_p = percentages_2;
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
    int total_sim;
    int k;
    _args *args = malloc(sizeof(_args));
    printf("Numbers of throw in total: \n");
    scanf("%d",&k);
    printf("Enter the toure number: \n");
    scanf("%d",&toure);
    printf("Enter the number of points the player 1 has: \n");
    scanf("%lld",&args->summ1);
    printf("Enter the number of points the player 2 has: \n");
    scanf("%lld",&args->summ2);
    printf("Number of experiments: \n");
    scanf("%d",&total_sim);
    //int sim_remains = total_sim % (thread_count - 1);
    //args->sim = total_sim/(thread_count - 1) + 1;
    args->sim = total_sim;
    if (k < toure){
        perror("The number of tours and/or the tour number are entered incorrectly\n");
        return 3;
    }
    if(k == toure){
        if(args->summ1 > args->summ2)
            printf("The first player will win with a 100 percent probability\n");
        else if (args->summ1 < args->summ2)
            printf("The second player will win with a 100 percent probability\n");
        else
            printf("draw\n");
        return 0;
    }
    if (args->summ1 > k*12 || args->summ2 > k * 12){
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
        _res* winn_percent;
        if (pthread_join(th[i],(void **) &winn_percent) != 0){
            perror("Failed phtread join \n");
            return 5;
        }
        printf("win first player percent: %d \nwin second player percent: %d\n \n",winn_percent->firs_p, winn_percent->second_p);
        free(winn_percent);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}