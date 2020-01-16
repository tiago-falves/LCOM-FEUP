extern uint32_t global_counter;
extern uint8_t status_code, scan_code;
int count_sysb;

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

  uint8_t config_value; // where the configuration is stored
  

  if (timer_get_conf(timer,&config_value) ||timer_display_conf(timer,config_value,field))
  {
    printf("timer_test_read_config error.\n");
    return 1;
  }
  
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  
  if(timer_set_frequency(timer,freq)){
    return 1;
  }
  return 0;
}

int(timer_test_int)(uint8_t time) {
  
  int ipc_status, r;
  message msg;
  uint8_t irq_set ;

  if(timer_subscribe_int(&irq_set)){
    printf("Error subscribing in lab2.\n");
    return 1;
  }

  while( global_counter < time*sys_hz() ) { 

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { // received notification
        
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
                       timer_int_handler();

                        if (global_counter % (int)sys_hz() == 0){
              timer_print_elapsed_time();

          } 
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else {
      
    }
 }

 if(timer_unsubscribe_int()){
    printf("Error unsubscribing in lab2.\n");
    return 1; 
 }


  return 0;
}






