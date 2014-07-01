#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
 
#include "transaction_ioctl.h"
 
void get_transaction(int fd)
{
    transaction_arg_t t;
 
    if (ioctl(fd, GET_TRANSACTION, &t) == -1)
    {
        perror("transaction_app ioctl get");
    }
    else
    {
        printf("\nCompra de valor : %f ", t.value);
        printf("feita por: %s\n", t.name);
        printf("O número do cartão é: %s ", t.number);
	printf("e o código encriptado: %d\n", t.code+CRYPT);
    }
}
void clr_transaction(int fd)
{
    if (ioctl(fd, CLR_TRANSACTION) == -1)
    {
        perror("transaction_app ioctl clr");
    }
}
void set_transaction(int fd)
{
    transaction_arg_t t;
    char *v[];
 	
    printf("Enter Name: ");
    scanf("%s", &v);
    strcpy(t.name, v);

    printf("Enter Card Number: ");
    scanf("%s", &v);
    strcpy(t.number, v);

    printf("Enter Card Code: ");
    scanf("%d", &t.code);

    printf("Enter Transaction Value: ");
    scanf("%f", &t.value);
 
    if (ioctl(fd, SET_TRANSACTION, &t) == -1)
    {
        perror("transaction_app ioctl set");
    }
}
 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/transaction";
    int fd;
    enum
    {
        e_get,
        e_clr,
        e_set
    } option;
 
    if (argc == 1)
    {
        option = e_get;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-g") == 0)
        {
            option = e_get;
        }
        else if (strcmp(argv[1], "-c") == 0)
        {
            option = e_clr;
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            option = e_set;
        }
        else
        {
            fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
        return 1;
    }
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("transaction_app open");
        return 2;
    }
 
    switch (option)
    {
        case e_get:
            get_transaction(fd);
            break;
        case e_clr:
            clr_transaction(fd);
            break;
        case e_set:
            set_transaction(fd);
            break;
        default:
            break;
    }
 
    close (fd);
 
    return 0;
}
