void main_default(void);
void tick_default(void);
void main_fs(void);
void tick_fs(void);

void wrapper(void){
main_default();
main_fs();
}
void tick_wrapper(void){
tick_default();
tick_fs();
}
