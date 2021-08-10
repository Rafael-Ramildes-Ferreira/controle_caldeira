//####################  Cabeçalho do Display  ####################//

void atualiza_valores_da_tela(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2,int index);

void inicializa_interface(struct atuador *lista1[],int lenght1,struct sensor *lista2[],int lenght2);

void print_warning(int valor);

void dont_print_warning();

int interpreta_escrita(double *T, double *H);
