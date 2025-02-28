#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h" 
#include "types.h"
#include "menu.h"
#define mx_vec_amount 100
#define mx_line_length 1000

static void value_parser(types* type_info, const char* str, void* dest) {
    if (type_info->type == INT) {
        *(int*)dest = atoi(str);
    } else if (type_info->type == DOUBLE) {
        *(double*)dest = atof(str);
    } else if (type_info->type == COMPLEX) {
        complex* c = dest;
        sscanf(str, "%lf,%lf", &c->real, &c->imag);
    }
}



cmd cmd_parser(const char *str_cmd) {
    const char *cmds[] = {"VEC_INIT", "GET_ELEM", "VEC_FILL", "VEC_FREE","SET_ELEM", "VEC_PRINTF",
         "VEC_FPRINTF", "SET_CONST_VEC","VEC_SUM", "VEC_SCAL_MULT","CUR_VECTORS_FPRINTF"};
    for (size_t i = 0; i < sizeof(cmds)/sizeof(cmds[0]); i++) {
        if (strcmp(str_cmd, cmds[i]) == 0) return (cmd)i;
    }
    return UNKNOWN_CMD;
}

void menu() {
    remove("output.txt");
    Vector *vectors[mx_vec_amount];
    size_t vectors_cnt = 0;
    char line[mx_line_length];
    while (fgets(line, sizeof(line), stdin)) { 
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '#' || strlen(line) == 0) continue;
        if (strlen(line) == 0)
            continue;
        char *tokens;
        char *str_cmd = strtok_r(line, " ", &tokens);
        if (!str_cmd) 
            continue;
        cmd current_cmd = cmd_parser(str_cmd);
        if (current_cmd == UNKNOWN_CMD) {
            printf("UNKNOWN COMMAND: %s\n", str_cmd);
            continue;
        }
        switch (current_cmd) {
            case VEC_INIT: {
                char *type_str = strtok_r(NULL, " ", &tokens);
                char *cap_str = strtok_r(NULL, " ", &tokens);
                if (!type_str || !cap_str) {
                    printf("VEC_INIT: ARGUMENT AMOUNT ERROR\n");
                    break;
                }
                datatype type;
                if (strcmp(type_str, "INT") == 0) 
                    type = INT;
                else if (strcmp(type_str, "DOUBLE") == 0)
                     type = DOUBLE;
                else if (strcmp(type_str, "COMPLEX") == 0)
                     type = COMPLEX;
                else {
                    printf("VEC_INIT: TYPE ERROR\n");
                    break;
                }
                char *endptr;
                size_t cap = strtoul(cap_str, &endptr, 10);
                if (cap_str == endptr || *endptr != '\0' || cap <= 0) {
                    printf("VEC_INIT: CAPACITY ERROR\n");
                    break;
                }
                Vector *v = vec_init(type, cap);
                if (!v) {
                    printf("VEC_INIT: INIT ERROR\n");
                    break;
                }
            
                if (vectors_cnt >= mx_vec_amount) {
                    printf("VEC_INIT: VECTOR COUNT ERROR\n");
                    vec_free(v);
                    break;
                }
            
                vectors[vectors_cnt++] = v;
                printf("VECTOR %d CREATED\n", vectors_cnt-1);
                break;
            }
            

            case GET_ELEM: {
                char *vec_idx = strtok_r(NULL, " ", &tokens);
                char *elem_idx = strtok_r(NULL, " ", &tokens);
                if (!vec_idx || !elem_idx) {
                    printf("GET_ELEM: INDEX ERROR\n");
                    break;
                }

                size_t v_i = (size_t)atoi(vec_idx);
                size_t e_i = (size_t)atoi(elem_idx);
                if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i] || e_i >= vectors[v_i]->capacity) {
                    printf("GET_ELEM: INDEX ERROR\n");
                    break;
                }

                void *elem = get_elem(vectors[v_i],e_i);
                vectors[v_i]->type_info->printf(elem);
                printf("\n");
                break;
            }

            case VEC_FILL: {
                char *vec_idx = strtok_r(NULL, " ", &tokens);
                if (!vec_idx) {
                    printf("VEC_FILL: INDEX ERROR\n");
                    break;
                }

                size_t v_i = (size_t)atoi(vec_idx);
                if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i]) {
                    printf("VEC_FILL: INDEX ERROR\n");
                    break;
                }

                Vector *v = vectors[v_i];
                char *rest = tokens;
                char *saveptr;
                for (size_t i = 0; i < v->capacity; i++) {
                    char *val_str = strtok_r(rest, " ", &saveptr);
                    rest = NULL;
                    if (!val_str) {
                        printf("VEC_FILL: NOT ENOUGH VALUES\n");
                        break;
                    }
                    value_parser(v->type_info, val_str, (char*)v->data + i * v->type_info->size);
                }
            break;
            }

            case VEC_FREE: {
                char *vec_idx = strtok_r(NULL, " ", &tokens);
                if (!vec_idx) {
                    printf("VEC_FREE: INDEX ERROR\n");
                    break;
                }
                size_t v_i = (size_t)atoi(vec_idx);
                if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i]) {
                    printf("VEC_FREE: INDEX INPUT ERROR\n");
                    break;
                }
                vec_free(vectors[v_i]);
                vectors[v_i] = NULL;
                break;
            }
            case SET_ELEM: {
                char *vec_idx = strtok_r(NULL, " ", &tokens);
                char *elem_idx = strtok_r(NULL, " ", &tokens);
                char *val_str = strtok_r(NULL, " ", &tokens);
                if (!vec_idx || !elem_idx || !val_str) {
                    printf("SET_ELEM: ARGUMENTS AMOUNT ERROR\n");
                    break;
                }

                size_t v_i = (size_t)atoi(vec_idx);
                size_t e_i = (size_t)atoi(elem_idx);   
                
                if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i] ) {
                    printf("SET_ELEM: INDEX ERROR\n");
                    break;
                }
                Vector* v = vectors[v_i];
                void* data = malloc(v->type_info->size);
                if (e_i >= vectors[v_i]->capacity) {
                    printf("SET_ELEM: INDEX ERROR\n");
                    free(data);
                    break;
                }
                value_parser(v->type_info, val_str, data);
                set_elem(v, e_i, data);
                free(data);
                break;
            }

            case VEC_PRINTF: {
                char *vec_idx = strtok_r(NULL, " ", &tokens);
                if (!vec_idx) {
                    printf("VEC_PRINTF: INDEX AMOUNT ERROR\n");
                    break;
                }

                int v_i = atoi(vec_idx);
                if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i]) {
                    printf("VEC_PRINTF: INDEX AMOUNT ERROR\n");
                    break;
                }
                vec_printf(vectors[v_i]);
                printf("\n");
                break;
            }
        case VEC_FPRINTF: {
            char *vec_idx = strtok_r(NULL, " ", &tokens);
            if (!vec_idx) {
                printf("VEC_FPRINTF: INDEX AMOUNT ERROR\n");
                break;
            }
        
            int v_i = atoi(vec_idx);
            if (v_i < 0 || v_i >= vectors_cnt || !vectors[v_i]) {
                printf("VEC_FPRINTF: INDEX ERROR\n");
                break;
            }
        
            FILE *f = fopen("output.txt", "a");
            if (!f) {
                printf("VEC_FPRINTF: CANNOT OPEN output.txt\n");
                break;
            }
            fprintf(f, "Vector %d: ", v_i); 
            vec_fprintf(vectors[v_i], f);
            fclose(f);
            break;
        }
            case SET_CONST_VEC: {
                char *changing = strtok_r(NULL, " ", &tokens);
                char *setting = strtok_r(NULL, " ", &tokens);
                if (!changing || !setting) {
                    printf("SET_CONST_VEC: INDEX AMOUNT ERROR\n");
                    break;
                }
                int chi = atoi(changing);
                int sti = atoi(setting);
                if (chi < 0 || chi >= vectors_cnt || !vectors[chi] || 
                    sti < 0 || sti >= vectors_cnt || !vectors[sti]) {
                    printf("SET_CONST_VEC: INDEX ERROR\n");
                    break;
                }

                set_const_vec(vectors[chi], vectors[sti]);
                break;
            }

            case VEC_SUM: {
                char *v1 = strtok_r(NULL, " ", &tokens);
                char *v2 = strtok_r(NULL, " ", &tokens);
                if (!v1 || !v2) {
                    printf("VEC_SUM: INDEX AMOUNT ERROR\n");
                    break;
                }

                int idx1 = atoi(v1);
                int idx2 = atoi(v2);
                if (idx1 < 0 || idx1 >= vectors_cnt || !vectors[idx1] || 
                    idx2 < 0 || idx2 >= vectors_cnt || !vectors[idx2]) {
                    printf("VEC_SUM: INDEX ERROR\n");
                    break;
                }

                Vector *sum = vec_sum(vectors[idx1], vectors[idx2]);
                if (!sum) {
                    printf("VEC_SUM: SUM ERROR\n");
                    break;
                }

                if (vectors_cnt >= mx_vec_amount) {
                    printf("VEC_SUM: VEC AMOUNT ERROR\n");
                    vec_free(sum);
                    break;
                }

                vectors[vectors_cnt++] = sum;
                printf("Sum vector: %d\n", vectors_cnt-1);
                break;
            }
            case VEC_SCAL_MULT: {
                char *v1 = strtok_r(NULL, " ", &tokens);
                char *v2 = strtok_r(NULL, " ", &tokens);
                if (!v1 || !v2) {
                    printf("VEC_SCAL_MULT: INDEX AMOUNT ERROR\n");
                    break;
                }
            
                int idx1 = atoi(v1);
                int idx2 = atoi(v2);
                if (idx1 < 0 || idx1 >= vectors_cnt || !vectors[idx1] || 
                    idx2 < 0 || idx2 >= vectors_cnt || !vectors[idx2]) {
                    printf("VEC_SCAL_MULT: INDEX ERROR\n");
                    break;
                }
            
                void *res = vec_scal_mult(vectors[idx1], vectors[idx2]);
                if (!res) {
                    printf("VEC_SCAL_MULT: PRODUCT ERROR\n");
                    break;
                }
            
                FILE *f = fopen("output.txt", "a");
                if (!f) {
                    printf("VEC_SCAL_MULT: CANNOT OPEN output.txt\n");
                    free(res);
                    break;
                }
            
                if (f) {
                    fprintf(f, "Scalar product of vectors %d and %d: ", idx1, idx2);
                    vectors[idx1]->type_info->fprintf(f, res); 
                    fprintf(f, "\n");
                    fclose(f);
                }
                printf("Scalar product: ");
                vectors[idx1]->type_info->printf(res);
                printf("\n");
                free(res);
                break;
            }
            case CUR_VECTORS_FPRINTF: {
                FILE *f = fopen("output.txt", "a");
                if (!f) {
                    printf("CUR_VECTORS_FPRINTF: CANNOT OPEN output.txt\n");
                    break;
                }
            
                fprintf(f, "\n====Current_vectors====\n");
                for (int i = 0; i < vectors_cnt; i++) {
                    if (vectors[i] != NULL) {
                        fprintf(f, "Vector %d: ", i);
                        vec_fprintf(vectors[i], f);
                    }
                }
                fprintf(f, "=======================\n\n");
                fclose(f);
                break;
            }   
            default:
                printf("UNKNOWN COMMAND\n");
        }
    }
    for (int i = 0; i < vectors_cnt; i++) {
        if (vectors[i]) vec_free(vectors[i]);
    }
}