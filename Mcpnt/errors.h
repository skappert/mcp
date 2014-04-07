extern char *erlst[];

/* Error Number Definitions*/
#define illegal_line               1
#define illegal_format             2
#define illegal_arithmetic         3
#define ambiguous                  4
#define illegal_delimiter          5
#define zero_divide                6
#define WA_full                    7
#define non_existent               8
#define wrong_type                 9
#define link_exhausted             10
#define not_properly               11
#define un_allocated               12
#define no_such_line               13
#define illegal_shuffle            14
#define if_error                   15
#define escape_typed               16
#define ask_error                  18
#define argument_list_error        20
#define file_error                 21

#define dimension_error            23
#define square_root_negative       24

#define log_negative               31       /* or = 0 */
#define device_not_connected       32
#define unauthorised_action        33
#define hardware_error             34
#define illegal_equipment_number   35
#define illegal_property           36
#define out_of_range               37
#define not_implemented            38
#define no_such_computer           39
#define string_filled              40
#define syntax_error               41
#define no_such_file               42
#define no_file_space              44
#define link_not_open              45
#define remitted_data_lost         46
#define end_of_file                47
#define def_area_full              52
#define def_syntax                 53
#define illegal_string_set         54
#define serr1                      54
#define string_function_failure    55
#define serr2                      55
#define Sif_error                  57       /* SERR4 on NORD */
#define Sask_error                 58       /* SERR5 */
#define string_expected            59       /* SERR6 */
#define no_link                    64
#define link_error                 65
#define code_failure               66
#define bracket_missing            67
#define wrong_hexa                 68
#define wrong_octal                69
#define illegal_character          70
#define illegal_operator           71
#define not_allowed                72
#define illegal_read_write         73
#define out_of_memory              74
#define database_not_loaded        75
#define illegal_command            76
#define resources_exhausted        77
#define file_not_open              78
#define camac_error                79    /*camac*/
#define no_such_loop               80    /*serial camac*/
#define simatic_error              81
#define driver_not_installed       82
#define port_not_available         83
#define scan_error                 84    /*scanner*/
#define no_high_voltage            85    /*tape station*/
#define non_existent_elem          86
#define non_existent_fec           87
#define non_existent_client        88
#define cannot_lock                89
#define not_running                90
#define not_posted                 91
#define not_accepted               92
#define dde_timeout                93
#define illegal_protocol           94
#define gpib_error                 95
#define rs232_error                96
#define Fug_in_Transition          97
#define connection_timeout         98
#define illegal_mode               99
#define illegal_plsline           100
#define not_defined               101
#define net_write_error           102
#define invalid_data              103
#define timer_too_low             104
#define access_denied             105
#define link_busy                 106
#define link_disabled             107
#define link_suspended            108
#define illegal_datatype		  109
#define profibus_error	          110
