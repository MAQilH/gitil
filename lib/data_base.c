#include "lib.h"
#include "../model/config_model.h"
#include <stdio.h>
#include <string.h>

char* get_creator(){
    Config loc = get_config(get_local_config_addres()), glo = get_config(get_global_config_addres());
    if(strlen(loc.name) == 0){
        return get_string_ref(glo.name);
    }
    return get_string_ref(loc.name);
}