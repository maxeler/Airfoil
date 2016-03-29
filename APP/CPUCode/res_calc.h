inline void res_calc(const double *dxx, const double *q1, const double *q2,
                     const double *adt1, const double *adt2, double *res1, double *res2) {
  double mu, ri, p1,vol1, p2,vol2, f,dx,dy;

  dx = dxx[0];
  dy = dxx[1];

  ri   = 1.0f/q1[0];
  p1   = gm1*(q1[3]-0.5f*ri*(q1[1]*q1[1]+q1[2]*q1[2]));
  vol1 =  ri*(q1[1]*dy - q1[2]*dx);

  ri   = 1.0f/q2[0];
  p2   = gm1*(q2[3]-0.5f*ri*(q2[1]*q2[1]+q2[2]*q2[2]));
  vol2 =  ri*(q2[1]*dy - q2[2]*dx);

//printf("%g %g\n",*adt1,*adt2);
//printf("%g %g %g %g\n",p1,p2,vol1,vol2);
  mu = 0.5f*((*adt1)+(*adt2))*eps;
  f = 0.5f*(vol1* q1[0]         + vol2* q2[0]        ) + mu*(q1[0]-q2[0]);
  //printf("%g ",f);
  res1[0] += f;
  res2[0] -= f;
  f = 0.5f*(vol1* q1[1] + p1*dy + vol2* q2[1] + p2*dy) + mu*(q1[1]-q2[1]);
  //printf("%g ",f);
  res1[1] += f;
  res2[1] -= f;
  f = 0.5f*(vol1* q1[2] - p1*dx + vol2* q2[2] - p2*dx) + mu*(q1[2]-q2[2]);
  //printf("%g ",f);
  res1[2] += f;
  res2[2] -= f;
  f = 0.5f*(vol1*(q1[3]+p1)     + vol2*(q2[3]+p2)    ) + mu*(q1[3]-q2[3]);
  //printf("%g ",f);
  res1[3] += f;
  res2[3] -= f;
  //printf("\n");
}

