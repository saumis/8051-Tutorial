#include<reg51.h>
sbit clk = P2^0; 
sbit din = P2^1;

code char seg[]={ 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x00,0x00,0x00,0x76,0x79,0x38,0x38,0x3F,0x3F,0x00,0x00};
void delay(unsigned int itime)
{
unsigned int z,k;
for(z=0;z<itime;z++)
for(k=0;k<1275;k++);
}

void main()
{
char j,i,dat;
while(1)
{
for(i=0;i<21;i++)
{
dat = seg[i];

for(j=0;j<8;j++)
{
din = (dat&(0x80>>j));
clk = 0;
clk = 1;
} 
delay(100);
}
}
}