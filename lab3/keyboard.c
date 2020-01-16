#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "i8042.h"
#include <stdint.h>

uint8_t status_code, scan_code_byte;
static int hook_id = 1;



//Tudo passado por variaveis globais
//ler o status do status register, ver se houve algum problema de ccomunicalao

//O interrupt handler le esse scancode do OUT BUFFER


//Deve se chekar estes dois no ih do Status Register 0x64
// 7 Parity Parity error - invalid data
// 6 Timeout Timeout error - invalid data



void (kbc_ih)(){


  if (util_sys_inb(STAT_REG, &status_code)){
    printf("Error reading status code");
    return;
  }
  
  if ((status_code & PARITY_BIT) | (status_code  & TIME_OUT_BIT)){
    printf("Parity or time out error");
    return;
  }

  if(util_sys_inb(OUTPUT_BUFFER, &scan_code_byte)){
    printf("Error reading scan code");
    return;
  }
  
}

//Keyboard IRQ is 1

int (keyboard_subscribe)(uint8_t *bit_no) {
    
    *bit_no = hook_id; 

    if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)){
      printf("Error subscribing notification.\n");
      return 1;
    }

  return 0;
}

int (keyboard_unsubscribe)() {
  
  if(sys_irqrmpolicy(&hook_id)){
    printf("Error unsubscribing notification.\n");
    return 1;
  }

  return 0;
}

//So u, byte de cada vez
//Os de dois bytes tem 0xE0 como primeiro byte
//Nao deve imprimir porque nao e reutilizavel
//Nao se deve escrever para o INBUF bit 1 se o IBF esta a 1

//int kbd_print_scancode(bool make, uint8_t size, uint8_t bytes[]);
//make -make or brak code
//size in bytes
//bytes od the scancode

//The difference between make code and break code is in the MSB
//E QUANDO O SCAN CODE SO TEM UM BYTE COMO FAZEMOS??  
//Porque com o 7 bit??
void print_full_scan_code(int *position){

  bool make = true;
  uint8_t size = 0;
  uint8_t bytes[2];
  
  bytes[*position] = scan_code_byte;
  if (!(scan_code_byte == INCOMPLETE_SCAN_CODE)){

    make = !((bool)(scan_code_byte & BIT(7)));
    size = *position +1;
    kbd_print_scancode( make, size, bytes);
    *position = 0;
  }
  else{
    *position +=1;
  }

}


// Command Meaning                  Args (A)/ Return (R)
// 0x20    Read Command Byte        Returns Command Byte
// 0x60    Write Command Byte       Takes A: Command Byte
// 0xAA    Check KBC (Self-test)    Returns 0x55, if OK
//                                  Returns 0xFC, if error
// 0xAB    Check Keyboard Interface Returns 0, if OK
// 0xAD    Disable KBD Interface
// 0xAE    Enable KBD Interface

//Os commandos devem ser escritos usando o adresso 0x64
//Na passagem de argumtnos deve ser usado 0x60


//Usar comando 0x20, escreve lo no 0x64 e tem de ser lido do 0x60

// 7 6  5   4   3 2  1   0
// – – DIS2 DIS – – INT2 INT
// DIS2 1: disable mouse
// DIS 1: disable keyboard interface
// INT2 1: enable interrupt on OBF, from mouse;
// INT 1: enable interrupt on OBF, from keyboard
//O KBC so e usado no poll?

// Read Use KBC command 0x20, which must be written to 0x64
// I But the value of the “command byte” must be read from
// 0x60
// Write Use KBC command 0x60, which must be written to 0x64
// I But the new value of the “command byte” must be
// written to 0x60

 int issue_kbc_command(uint8_t command_byte){

    while( 1 ) {
      if (util_sys_inb(STAT_REG, &status_code)){
        printf("Error reading status code \n");
        return 1;
      }
 
      if( (status_code & IBF_BIT) == 0 ) {

        if(sys_outb(KBC_COMMAND_REG, WRITE_COMMAND_BYTE)){
          printf("Error sending write command \n");
          return 1;
        }
        if(sys_outb(KBC_ARGUMENT_REG,command_byte)){
          printf("Error sending argument \n");
          return 1;
        }
        return 0;
      }
      tickdelay(micros_to_ticks(DELAY_US));
    }
 }

uint8_t read_kbc_data(){

    uint8_t cmd;

    while( 1 ) {

      util_sys_inb(STAT_REG, &status_code); /* assuming it returns OK */

      if ( sys_outb(KBC_COMMAND_REG,READ_COMMAND_BYTE)){
          printf("Error sending command\n");
          return 1;
        }
      /* loop while 8042 output buffer is empty */

      if( status_code & OBF_BIT ) {
        
        
       if (util_sys_inb(OUTPUT_BUFFER, &cmd)){
         printf("Error reading command");
         return 1;
       }
      
        if ( (status_code & (PARITY_BIT | TIME_OUT_BIT)) == 0 )  return cmd;
        else  return -1;
      }
      tickdelay(micros_to_ticks(DELAY_US));
    }
}


void reenable_keyboard_interrupt(){
  uint8_t command = read_kbc_data();
  command |= INT0;
  issue_kbc_command(command);
}
