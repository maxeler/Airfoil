inline void adt_calc(const double *dx0, const double *dx1, const double *dx2, const double *dx3, const double *q,double *adt){
  double dx,dy, ri,u,v,c;

  ri =  1.0f/q[0];
  u  =   ri*q[1];
  v  =   ri*q[2];
  c  = sqrt(gam*gm1*(ri*q[3]-0.5f*(u*u+v*v)));

  dx = dx0[0];
  dy = dx0[1];
  *adt  = fabs(u*dy-v*dx) + c*dx0[2];
  //printf("%g %g %g %g %g %g %g\n",dx,dy,u,v,fabs(u*dy-v*dx),sqrt((u*dy-v*dx)*(u*dy-v*dx)),sqrt(u*u*dy*dy + v*v*dx*dx - 2*u*v*dx*dy));

//printf("%g,%g\n",dx,dy);
  dx = dx1[0];
  dy = dx1[1];
  *adt += fabs(u*dy-v*dx) + c*dx1[2];

//printf("%g,%g\n",dx,dy);
  dx = dx2[0];
  dy = dx2[1];
  *adt += fabs(u*dy-v*dx) + c*dx2[2];

//printf("%g,%g\n",dx,dy);
  dx = dx3[0];
  dy = dx3[1];
  *adt += fabs(u*dy-v*dx) + c*dx3[2];
//printf("%g,%g\n",dx,dy);
//printf("\n\n");
  *adt = (*adt) / cfl;

//printf("%g\n",*adt);
}

