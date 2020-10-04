
typedef  double** m;

void MInput(m*,unsigned, unsigned);

void MOutput(m ,unsigned, unsigned);

void MDel(m ,unsigned );

void MSumm(m ,m ,unsigned, unsigned);

void MMulI(double, m, unsigned, unsigned);

void MMul(m*, m, unsigned);

void GetM(m *, m, int, int, int);

double MDet(m, unsigned);
