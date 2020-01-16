#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "i8254.h"

#include <stdbool.h>
#include <stdint.h>

#define ESC_BREAK_CODE 0x81

extern uint8_t status_code, scan_code_byte;
extern uint32_t global_counter;

int count_sysb;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


//Quando se clica numa tecla do teclado envia-se
//scancodes para o KBC(Keybiard controller)
//O KBC poe num registo e gera um interrupt





//Command byte



//Os breakcodes tem o MSB set (Make 0x01 Break =x81)
//A maior parte so tem um byte mas ha algumas com mais
//Quando e premida uma tecla, o scan code é posto no buffer para se mandar para o pc
//Quando o Out Buffer esta vazio:
  //O Kbc aviza através do serial bus
  //The KBD sends the byte at the end 0f the buffer to the kbc
  //The KBC puts it in the OUT BUFFER 
  //O kbc gera um interrupt by raising IRQ1

//Dar subscribe nao so usando o IRQReenable como o IRQ Exclusive
//Usar a funçao tickdelay entre interrupts

//Sempre qie o ih receber um byte do keyboard,deve tentar 
//fazer um scnacode, e imprimi lo na consola

//Tem que se desativar o ih do minix antes de usarmos o nosso



//deve sair com ESC = 0x81
//Unsubscribe antes de terminar
//print number of sys_inb




int(kbd_test_scan)() {

  int ipc_status, r;
  int position = 0;
  message msg;
  uint8_t irq_set_keyboard ;

  if(keyboard_subscribe(&irq_set_keyboard)){
    printf("Error subscribing in lab2.\n");
    return 1;
  }
 
  while(scan_code_byte != ESC_BREAK_CODE) { 

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    
    if (is_ipc_notify(ipc_status)) { // received notification
        
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
              if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) { /* subscribed interrupt */
                kbc_ih();
                print_full_scan_code(&position);       
              }
        
              break;
          default:
              break; /* no other notifications expected: do nothing */	
      }
    } else {
      
    }
 }

  if(keyboard_unsubscribe()){
      printf("Error unsubscribing in lab2.\n");
      return 1; 
 }


  return 0;
  
}

//Nao se pode usar interrupts
//Disable interrupts
//Read the scancodes
//Print them on the console
//Deve sair quando a tecla esc é carregada 
//Antes de sair deve voltar a dar enable aos interrupts
//Isso e feito a escrever um KBC command byte
//Esta e a razao pela qual o keyboard pode ficar sem efeito

//Read the obf only if obf is set and aux is cleared
//Desenhar uma solução que faça com aciete qualque commando do KBC nao so o 0x20 e 0x60
//Escrever funcoes baixo nivel que leiam e escrebam KBC registers
//Que lidem cim KBC HW interface, como ver se a flag IBF esta cheia

//Alto nivel Escever paramteros conforme necessario, sabe o protocolo e os comandos
//Status register, usado para ker o KBC state
// The values from A2 are used to distinguish Kbc and kbd commands

//Status Register 0x64
//Iput buffer - Commands to the kbs - 0x64
//            - Argumetns of KBC COMMANDs 0x60
//Output buffer lido do 0x60

//Nao escrever ao input buffer caso ele ja esteja setted



// 7 Parity Parity error - invalid data
// 6 Timeout Timeout error - invalid data
// 5 Aux Mouse data
// 4 INH Inhibit flag: 0 if keyboard is inhibited
// 3 A2 A2 input line: 0 data byte
// 1 command byte
// 2 SYS System flag: 0 if system in power-on reset,
// 1 if system already initialized
// 1 IBF Input buffer full
// don’t write commands or arguments
// 0 OBF Output buffer full - data available for reading


int(kbd_test_poll)() {

  int position = 0;


  while(scan_code_byte != ESC_BREAK_CODE) { 

    if (util_sys_inb(STAT_REG, &status_code)){
      printf("Error reading status code");
      return 1;
    }

    if (((OBF_BIT & status_code) == 0) || (AUX_BIT & status_code) != 0){
      continue;
    }
    
    
    if ((status_code & PARITY_BIT) | (status_code  & TIME_OUT_BIT)){
      printf("Parity or time out error");
      return 1;
    }

    if(util_sys_inb(OUTPUT_BUFFER, &scan_code_byte)){
      printf("Error reading scan code");
      return 1;
    }  
    

    print_full_scan_code(&position);
    tickdelay(micros_to_ticks(DELAY_US));

  }

  reenable_keyboard_interrupt();
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  
  int ipc_status, r;
  int position = 0;
  message msg;
  uint8_t irq_set_keyboard,irq_set_timer ;

  if (timer_subscribe_int(&irq_set_timer))
  {
    printf("Error subscribing timer.\n");
    return 1;
  }
  

  if(keyboard_subscribe(&irq_set_keyboard)){
    printf("Error subscribing keyboard.\n");
    return 1;
  }


  while((scan_code_byte != ESC_BREAK_CODE) && ((global_counter / sys_hz()) < n)) { 

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { // received notification
        
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */				
              if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) { /* subscribed interrupt */
                kbc_ih();
                print_full_scan_code(&position);
                global_counter =0;
                
              }
              if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed interrupt */
                timer_int_handler(); 
                

              }
              break;
          default:
              break; /* no other notifications expected: do nothing */	
      }
    } else {
      
    }
 }

  if(keyboard_unsubscribe()){
      printf("Error unsubscribing in lab2.\n");
      return 1; 
 }
  if(timer_unsubscribe_int()){
    printf("Error unsubscribing timer.\n");
    return 1; 
 }


  return 0;
  
}
