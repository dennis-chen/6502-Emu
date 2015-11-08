 /* file: minunit.h */
 #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
 #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
 #define mu_run_test_with_args(test,arg1,arg2,arg3) do { char *message = test(arg1,arg2,arg3); tests_run++; \
                                if (message) return message; } while (0)
 extern int tests_run;
