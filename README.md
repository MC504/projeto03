projeto03
=========

Projeto 03 - MC504 Unicamp - 1s2014

-Rodar os seguintes comandos para compilar em 32bits:
export CFLAGS=-m32
export LDFLAGS=-m32

-Rodar make na pasta do driver dentro do Linux para compilar o programa de teste.

-Rodar make na pasta do Linux para compilar os drivers (.ko)

-Usar o debugfs para colocar os arquivos .ko e o executavel do programa de teste em um filesystem

-Rodar o seguinte comando para iniciar o emulador:
qemu-system-i386 -hda mc504.img -kernel linux-3.14.4/arch/i386/boot/bzImage -append "ro root=/dev/hda" -hdb drivers.img


-Dentro do SO emulado, montar o filesystem (que fica no /dev/hdb)

-Carregar o módulo usando insmod no arquivo .ko

-Rodar o programa de teste com as flags:
  -s para escrever
  -g para ler
  -c para limpar
