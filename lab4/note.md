## shm_open
 создает или/и открывает POSIX общую область памяти(POSIX shared memory object). Posix shared memory object - по сути дескриптор, который может быть использован для несвязных процессов для mmap одной и той же области выделеной памяти


``` 
shmFd = shm_open(SHARED_OBJ_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); 
``` 

в нашем  случае мы создаем объект(файловый  дискриптор) имени SHARED_OBJ_NAME(char[]), O_CREAT | O_RDWR(создаст данный объект в случае отсутствия/откроет на чтение и запись), S_IRUSR | S_IWUSR(Чтение и запись для юзера)
в случае не ндобности мы можем просто закрыть его
```
close(shmFd)
```
Чтобы удалить данную shared memory воспользуемся
```
shm_unlink(SHARED_OBJ_NAME);
```

## ftruncate
выделяем/режем память под наш объект
```
ftruncate(shmFd, sizeof(message))
```
Выделяем память нашему файлу в размере message(где message структура созданная ранее)


## mmap
отображает некоторую область памяти на память выбранного процесса 
```
message *msg_ptr = (message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
```
в нашем случае ядро выбирает наш процесс(в котором мы создали mmap), т к *addr NULL, размер выделенной памяти sizeof(message), PROT_READ | PROT_WRITE - с нашей памяти можно читать и писать,  MAP_SHARED - память общая, shmFd файловый дискриптор, который мы отображаем, 0 откуда мы начинаем отображать наш shmFd.

Файловый дискриптор можно сразу же закрыть после отображения.

### Прекращение отображения
```
munmap(msg_ptr, sizeof(message));
```