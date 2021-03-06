#include "pep.h"

double vec_sum(const double *vec, int len) {
  int i;
  double res;
  res=vec[0];
  for(i=1;i<len;i++) res+=vec[i];
  return res;
}

double vec_max(const double *vec, int len) {
  int i;
  double res;
  res=vec[0];
  for(i=1;i<len;i++) {
    if(res<vec[i]) res=vec[i];
  }
  return res;
}

double vec_min(const double *vec, int len) {
  int i;
  double res;
  res=vec[0];
  for(i=1;i<len;i++) {
    if(res>vec[i]) res=vec[i];
  }
  return res;
}

double vec_mean(const double *vec, int len) {
  double tmp=0.0;
  int i;
  for(i=0;i<len;i++) tmp+=vec[i];
  tmp=tmp/((double) len);
  return tmp;
}

double vec_var(const double *vec, int len) {
  double mean=0.0;
  double var=0.0;
  int i;
  for(i=0;i<len;i++) mean+=vec[i];
  mean=mean/((double) len);
  for(i=0;i<len;i++) var+=pow((vec[i]-mean),2);
  var/=((double) (len-1));

  return var;
}

/*** only using IQR ***/
void vec_var2(const double *vec, int len, double *mean, double *var) {
  double m=0.0;
  double v=0.0;
  int i, start, end, newlen;
  double new_vec[len];

  start = (int) ( ((double) len) * 0.05  );
  end = (int) ( ((double) len) * 0.95  );
  newlen = end - start + 1;

  for(i=0;i<len;i++) new_vec[i]=vec[i]; 
  gsl_sort(new_vec,1,len);

  m = 0.0;
  for(i=start;i<=end;i++) m+=new_vec[i];
  m /= ( (double) newlen );

  v = 0.0;
  for(i=start;i<=end;i++) v+=pow((new_vec[i]-m),2);
  v /= ((double) (newlen-1));
  
  *mean = m;
  *var = v;

}


double vec_med(const double *vec, int len)
{
  double new_vec[len];
  double med;
  int i, pk;
  for(i=0;i<len;i++) {
    new_vec[i]=vec[i]; 
  }
  if(len==1) {
    med=vec[0];
    return med;
  }
  else if(len%2==0) {
    gsl_sort(new_vec,1,len);
    pk=(len-2)/2;
    med=(new_vec[pk]+new_vec[pk+1])/2.0;
    return med;
  }
  else {
    gsl_sort(new_vec,1,len);
    pk=(len-1)/2;
    med=new_vec[pk];
    return med;
  }
}

double vec_mad(const double *vec, int len)
{
  double new_vec[len];
  double med, mad;
  int i;

  med = vec_med(vec,len);
  for(i=0;i<len;i++) {
    new_vec[i]=fabs(vec[i] - med); 
  }
  mad = vec_med(new_vec,len);
  return mad;
}



int ranMultinom(double *p, int K) {
  int i, rr;
  double coin, sum; 
  sum = vec_sum(p,K);
  /*for(i=0;i<K-1;i++) fprintf(stderr, "%.2f\t", p[i]);
  fprintf(stderr, "%.2f\n", p[K-1]); */
  if(sum != 1.0) {
    for(i=0;i<K;i++) p[i] /= sum;
  }
  coin = gsl_ran_flat(r,0.0,1.0);
  sum = p[0];
  rr = 0;
  while(coin > sum) {
    rr++;
    sum += p[rr];
  }
  if(rr >= K) rr = K-1;
  return rr;
}



double fmin(double a, double b) {
  double res;
  res = a > b ? b : a;
  return res;
}
