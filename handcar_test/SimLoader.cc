//
// Copyright (C) [2020] Futurewei Technologies, Inc.
//
// FORCE-RISCV is licensed under the Apache License, Version 2.0 (the
// "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE. See the
// License for the specific language governing permissions and limitations under
// the License.
//

#include "SimLoader.h"

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//!< check for error when any simulator function address is loaded:
int CheckSimOp(const char *which_op);

//!< load the simulator shared object, get api function addresses:
int open_sim_dll(const char *sim_dll_path, struct SimDllApi *api_ptrs) {
    //!< load the simulator dll...
    void *my_sim_lib = (*api_ptrs).sim_lib = dlopen(sim_dll_path, RTLD_LAZY);
    char *errmsg = dlerror();
    if (errmsg != NULL) {
        fprintf(
            stderr,
            "ERROR: Problems loading simulator DLL. Error msg from dlopen: "
            "'%s'\n",
            errmsg);
        return -1;
    }

    //!< simulator is loaded. now get function addresses...

    (*api_ptrs).initialize_simulator =
        (void (*)(int, char **))dlsym(my_sim_lib, "initialize_simulator");
    if (CheckSimOp("initialize_simulator")) return -1;

    // (*api_ptrs).terminate_simulator =
    //     (void (*)())dlsym(my_sim_lib, "terminate_simulator");
    // if (CheckSimOp("terminate_simulator")) return -1;

    // (*api_ptrs).simulator_load_elf =
    //     (int (*)(int, const char *))dlsym(my_sim_lib, "simulator_load_elf");
    // if (CheckSimOp("simulator_load_elf")) return -1;

    (*api_ptrs).step_simulator =
        (void (*)(int, int, int))dlsym(my_sim_lib, "step_simulator");
    if (CheckSimOp("step_simulator")) return -1;

    // (*api_ptrs).get_disassembly =
    //     (int (*)(const uint64_t *, char **, char **))dlsym(
    //         my_sim_lib, "get_disassembly");
    // if (CheckSimOp("get_disassembly")) return -1;

    // (*api_ptrs).get_simulator_version =
    //     (int (*)(char *))dlsym(my_sim_lib, "get_simulator_version");
    // if (CheckSimOp("get_simulator_version")) return -1;

    (*api_ptrs).read_simulator_register =
        (int (*)(int, const char *, uint8_t *, int))dlsym(
            my_sim_lib, "read_simulator_register");
    if (CheckSimOp("read_simulator_register")) return -1;

    // (*api_ptrs).partial_read_large_register =
    //     (int (*)(int, const char *, uint8_t *, uint32_t, uint32_t))dlsym(
    //         my_sim_lib, "partial_read_large_register");
    // if (CheckSimOp("partial_read_large_register")) return -1;

    // (*api_ptrs).partial_write_large_register =
    //     (int (*)(int, const char *, const uint8_t *, uint32_t,
    //     uint32_t))dlsym(
    //         my_sim_lib, "partial_write_large_register");
    // if (CheckSimOp("partial_write_large_register")) return -1;

    (*api_ptrs).write_simulator_register =
        (int (*)(int, const char *, const uint8_t *, int))dlsym(
            my_sim_lib, "write_simulator_register");
    if (CheckSimOp("write_simulator_register")) return -1;

    (*api_ptrs).read_simulator_memory =
        (int (*)(int, const uint64_t *, int, uint8_t *))dlsym(
            my_sim_lib, "read_simulator_memory");
    if (CheckSimOp("read_simulator_memory")) return -1;

    (*api_ptrs).write_simulator_memory =
        (int (*)(int, const uint64_t *, int, const uint8_t *))dlsym(
            my_sim_lib, "write_simulator_memory");
    if (CheckSimOp("write_simulator_memory")) return -1;

    // (*api_ptrs).translate_virtual_address =
    //     (int (*)(int, const uint64_t *, int, uint64_t *, uint64_t *))dlsym(
    //         my_sim_lib, "translate_virtual_address");
    // if (CheckSimOp("translate_virtual_address")) return -1;

    // (*api_ptrs).initialize_simulator_memory =
    //     (int (*)(int, const uint64_t *, int, uint64_t))dlsym(
    //         my_sim_lib, "initialize_simulator_memory");
    // if (CheckSimOp("initialize_simulator_memory")) return -1;

    //!< other simulator functions T

    return 0;
}

//!< close sim dll (if open)...
void close_sim_dll(struct SimDllApi *api_ptrs) {
    if ((*api_ptrs).sim_lib != NULL) {
        dlclose((*api_ptrs).sim_lib);
        (*api_ptrs).sim_lib = NULL;
    }
}

//!< check for error when any simulator function address is loaded:

int CheckSimOp(const char *which_op) {
    char *errmsg = dlerror();
    if (errmsg != NULL) {
        fprintf(
            stderr,
            "ERROR: Problems loading simulator DLL '%s' symbol. Error msg from "
            "dlsym: '%s'\n",
            which_op, errmsg);
        return -1;
    }
    return 0;
}
