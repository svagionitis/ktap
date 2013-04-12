#ifndef __KTAP_H__
#define __KTAP_H__

#include "ktap_types.h"
#include "ktap_opcodes.h"

#include <linux/trace_seq.h>

typedef struct ktap_Reg {
        const char *name;
        ktap_cfunction func;
} ktap_Reg;

ktap_State *ktap_newstate(ktap_State **ks);
void ktap_exit(ktap_State *ks);
ktap_State *ktap_newthread(ktap_State *mainthread);
void ktap_exitthread(ktap_State *ks);
Closure *ktap_load(ktap_State *ks, unsigned char *buff);
void ktap_call(ktap_State *ks, StkId func, int nresults);
void ktap_optimize_code(ktap_State *ks, int level, Proto *f);
void ktap_register_lib(ktap_State *ks, const char *libname, const ktap_Reg *funcs);

void ktap_init_baselib(ktap_State *ks);
void ktap_init_oslib(ktap_State *ks);

void ktap_event_handle(ktap_State *ks, void *e, int field, StkId ra);
int ktap_trace_init(ktap_State *ks);
void ktap_trace_exit(ktap_State *ks);
int ktap_probe_init(ktap_State *ks);
void ktap_probe_exit(ktap_State *ks);

int start_trace(ktap_State *ks, const char *event_name, Closure *cl);
int start_probe(ktap_State *ks, const char *event_name, Closure *cl);
void end_all_trace(ktap_State *ks);
void end_probes(ktap_State *ks);
int ktap_event_get_index(const char *field);
Tstring *ktap_event_get_ts(ktap_State *ks, int index);

int ktap_strfmt(ktap_State *ks, struct trace_seq *seq);

void ktap_transport_write(ktap_State *ks, const void *data, size_t length);
void *ktap_transport_reserve(ktap_State *ks, size_t length);
void ktap_transport_exit(ktap_State *ks);
int ktap_transport_init(ktap_State *ks);

void ktap_user_complete(ktap_State *ks);

void trace_console_func(void *__data, const char *text, size_t len);
void ktap_exit_timers(ktap_State *ks);

extern DEFINE_PER_CPU(bool, ktap_in_tracing);

/* this structure allocate on stack */
struct ktap_trace_arg {
	ktap_State *ks;
	Closure *cl;
};

/* this structure allocate on stack */
struct ktap_event {
	struct ftrace_event_call *call;
	void *entry;
	int entry_size;
};

typedef void (*ftrace_call_func)(struct ftrace_event_call * call, void *data);
void ftrace_on_event_call(const char *buf, ftrace_call_func actor, void *data);

void ktap_call_probe_closure(ktap_State *mainthread, Closure *cl,
			     struct ktap_event *event);
#endif /* __KTAP_H__ */
