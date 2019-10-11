/*
 Para programar los PIC16F882, 883, 884, 886 y 887
 Probado con un PIC16F887
 Espera como argumentos el puerto y cÃ³digo HEX a utilizar
 Ejemplo para programar a travÃ©s de adaptador USB-RS232: 
   ./Prog88x /dev/ttyUSB0 prueba.hex
 Se compila con:
   gcc Prog88xV2.c -o Prog88x $(pkg-config --cflags --libs gtk+-3.0)
 Si requieres instalar gtk, puedes usar la instrucciones siguientes:
   sudo apt update
   sudo apt install libgtk-3-dev
 CodificaciÃ³n: UTF-8
 CC BY-NC Alfonso GutiÃ©rrez Aldana, septiembre de 2017
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int puerto;
void ModoProg(void);
void Escribe(int comando);
void Escribe2(int comando, int dato);
unsigned short int Lee(int comando);
void LeeIdConfig(void);
void BorraMemProg(void);
void ProgMemProg(FILE *arch);
void LeeMemProg(void);

void main(int nargs, char *args[])
{
 struct termios atributos;
 FILE *archivo;

 if (nargs!=3)
 {
  printf("\nEjemplo:\n./Prog /dev/ttyUSB0 prog.hex\n\n");
  exit(1);
 }
 puerto=open(args[1],O_RDWR|O_NOCTTY); // Abre el puerto
 if (puerto<0)
 {
  printf("\nFalla al intentar abrir el puerto %s\n",args[1]);
  printf("Ejemplo:\n./Prog /dev/ttyUSB0 prog.hex\n\n");
  exit(1);
 }
 archivo=fopen(args[2],"rt");
 if (archivo==NULL)
 {
  printf("\nFalla al intentar abrir el archivo .hex\n\n");
  close(puerto);
  exit(1);
 } 
 tcgetattr(puerto,&atributos); // Lee configuraciÃ³n del puerto
 atributos.c_lflag&=(~ICANON); // Elimina los caracteres de control
 atributos.c_lflag&=(~ECHO); // Elimina el eco
 // Habilita lectura e ignora lineas de control, 8 bits a 19200 bps
 atributos.c_cflag=B19200|CS8|CREAD|CLOCAL;
 // Regresa inmediatamente de la operaciÃ³n de lectura con lo que encontrÃ³
 atributos.c_cc[VMIN]=0; atributos.c_cc[VTIME]=0;
 // Permitir la transmisiÃ³n y recepciÃ³n de los cÃ³digos del 0 al 255
 atributos.c_iflag&=~ICRNL;
 atributos.c_iflag&=~IXOFF;
 atributos.c_iflag&=~IXON;
 atributos.c_lflag&=~ISIG;
 atributos.c_oflag&=~OPOST;
 tcsetattr(puerto,TCSANOW,&atributos); // Establece la nueva configuraciÃ³n
 LeeIdConfig();
 BorraMemProg();
 ProgMemProg(archivo);
 LeeMemProg();
 LeeIdConfig();
 close(puerto); // Cierra el puerto y el archivo
 fclose(archivo);
}

void ModoProg(void)
{
 unsigned char modoProgamacion=0;
 
 write(puerto,&modoProgamacion,1);
 g_usleep(600);
}

void Escribe(int comando)
{
 int n, control=255;
 
 for (n=0; n<6; n++)
 {
  if (comando&0x1) control&=~TIOCM_DTR; else control|=TIOCM_DTR;
  comando=comando>>1;
  control&=~TIOCM_RTS; ioctl(puerto,TIOCMSET,&control); // RTS a 1
  control|=TIOCM_RTS;  ioctl(puerto,TIOCMSET,&control); // RTS a 0
 }
}

void Escribe2(int comando, int dato)
{
 int n, control=255;
 
 for (n=0; n<6; n++)
 {
  if (comando&0x1) control&=~TIOCM_DTR; else control|=TIOCM_DTR;
  comando=comando>>1;
  control&=~TIOCM_RTS; ioctl(puerto,TIOCMSET,&control); // RTS a 1
  control|=TIOCM_RTS;  ioctl(puerto,TIOCMSET,&control); // RTS a 0
 }
 dato=dato<<1;
 for (n=0; n<16; n++)
 {
  if (dato&0x1) control&=~TIOCM_DTR; else control|=TIOCM_DTR;
  dato=dato>>1;
  control&=~TIOCM_RTS; ioctl(puerto,TIOCMSET,&control); // RTS a 1
  control|=TIOCM_RTS;  ioctl(puerto,TIOCMSET,&control); // RTS a 0
 }
}

unsigned short int Lee(int comando)
{
 int n, dato=0, control=255;
 
 for (n=0; n<6; n++)
 {
  if (comando&0x1) control&=~TIOCM_DTR; else control|=TIOCM_DTR;
  comando=comando>>1;
  control&=~TIOCM_RTS; ioctl(puerto,TIOCMSET,&control); // RTS a 1
  control|=TIOCM_RTS;  ioctl(puerto,TIOCMSET,&control); // RTS a 0
 }
 for (n=0; n<16; n++)
 {
  dato>>=1;
  control&=~TIOCM_RTS; ioctl(puerto,TIOCMSET,&control); // RTS a 1
  control|=TIOCM_RTS;  ioctl(puerto,TIOCMSET,&control); // RTS a 0
  ioctl(puerto,TIOCMGET,&control);
  if (!(control&TIOCM_CTS)) dato|=0x8000;
 }
 dato>>=1;
 return dato;
}

void LeeIdConfig(void)
{
 int n;
 
 ModoProg();
 Escribe2(0x0,0x0); // comando cargar configuraciÃ³n
 for (n=0; n<6; n++) Escribe(0x6); // comando incrementar direcciÃ³n
 printf("identificador: %X\n",Lee(0x4)); // imprime lo que lee de la memoria de programa
 Escribe(0x6);
 printf("bits de configuraciÃ³n: %X\n",Lee(0x4));
 Escribe(0x6);
 printf("                     : %X\n",Lee(0x4));
}

void BorraMemProg(void)
{
 ModoProg();
 Escribe(0x9); // comando borrar memoria de programa
 g_usleep(6100); // espera el tiempo necesario para borrar
}

void LeeMemProg(void)
{
 int n, m, dato;
 
 ModoProg();
 for (n=0; n<10; n++)	// Para leer toda la memoria reemplazar el 10 por 256
 {
  printf("%04X > ",n*16);
  for (m=0; m<8; m++)
  {
   dato=Lee(0x4);
   dato=((dato&0xFF00)>>8)|((dato&0x00FF)<<8);
   printf("%04X ",dato);
   Escribe(0x6);
  }
  printf("\n");
 }
}

void ProgMemProg(FILE *arch)
{
 char renglon[50], cadena[5]="____";
 int n, m, dir, dato, n_datos, direc, direc_llenado, saltar;
 
 ModoProg();
 direc_llenado=0;
 do
 {
  fgets(renglon,50,arch);
  printf("%s",renglon);
  if (renglon[8]=='0')
  {
   cadena[0]=renglon[3]; cadena[1]=renglon[4]; cadena[2]=0;
   dir=(int)strtol(cadena,NULL,16); // para distinguir entre instrucciones y bits de configuraciÃ³n
   if (dir<0x40)
   {
    cadena[0]=renglon[1]; cadena[1]=renglon[2]; cadena[2]=0;
    n_datos=(int)strtol(cadena,NULL,16)/2; // nÃºmero de instrucciones en el renglÃ³n
    cadena[0]=renglon[3]; cadena[1]=renglon[4]; cadena[2]=renglon[5]; cadena[3]=renglon[6]; cadena[4]=0;
    direc=(int)strtol(cadena,NULL,16)/2; // direcciÃ³n inicial de programaciÃ³n
    if (direc>direc_llenado) saltar=direc-direc_llenado; else saltar=0;
    for (m=0; m<saltar; m++) Escribe(0x06); // En caso de que las direcciones NO sean consecutivas
    direc_llenado=direc+n_datos;
    for (m=0; m<n_datos; m++)
    {
     cadena[0]=renglon[11+m*4]; cadena[1]=renglon[12+m*4];
     cadena[2]=renglon[9+m*4]; cadena[3]=renglon[10+m*4];
     dato=(int)strtol(cadena,NULL,16);
     Escribe2(0x2,dato);
     Escribe(0x8);
     g_usleep(6100);
     Escribe(0x6);
    }
   }
   if (dir==0x40) // Programa los bits de configuraciÃ³n
   {
    cadena[0]=renglon[11]; cadena[1]=renglon[12];
    cadena[2]=renglon[9]; cadena[3]=renglon[10];
    dato=(int)strtol(cadena,NULL,16);
    ModoProg();
    Escribe2(0x0,0x0);
    if (renglon[5]=='0') // CONFIG1
    {
     for (n=0; n<7; n++) Escribe(0x6);
     dato|=0x01020; // Activa la programaciÃ³n a bajo voltaje y el uso de MCLR
     Escribe2(0x2,dato);
    }
    else // CONFIG2
    {
     for (n=0; n<8; n++) Escribe(0x6);
     Escribe2(0x2,dato);
    }
    Escribe(0x8);
    g_usleep(6100);
   }
  }
 } while(renglon[8]!='1'); // Detecta la Ãºltima lÃ­nea del archivo 
}