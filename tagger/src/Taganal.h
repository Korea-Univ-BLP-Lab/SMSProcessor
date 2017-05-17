#ifndef TAGANAL_H
#define TAGANAL_H

/*--------------------------------------------------------------*\
  분석에 오류가 없는지 검사하고 형태소의 개수를 반환한다.
  반환값 : 형태소의 개수, -1(error)
\*--------------------------------------------------------------*/
int chkanl (char *Anl);

char *anltok (char *Anl);

char *anltok_r (char *Anl, char **last);

char *GetPosHead (char *Anl, char *Head);

char *GetPosTail (char *Anl, char *Tail);

#endif
