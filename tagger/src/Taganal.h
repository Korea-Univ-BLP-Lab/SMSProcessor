#ifndef TAGANAL_H
#define TAGANAL_H

/*--------------------------------------------------------------*\
  �м��� ������ ������ �˻��ϰ� ���¼��� ������ ��ȯ�Ѵ�.
  ��ȯ�� : ���¼��� ����, -1(error)
\*--------------------------------------------------------------*/
int chkanl (char *Anl);

char *anltok (char *Anl);

char *anltok_r (char *Anl, char **last);

char *GetPosHead (char *Anl, char *Head);

char *GetPosTail (char *Anl, char *Tail);

#endif
