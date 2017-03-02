/*
 * qstrstr.c
 *
 *  Created on: 2016. 12. 13.
 *      Author: root
 */


#include <stdio.h>
#include <string.h>



int qstrstr(char *buf, size_t buflen, const char *match)
{
	int i;
	int match_len = strlen(match);
	for (i=0; i<buflen; i++)
	{
		if (match[0] == buf[i])
		{
			if (match_len > 3)
			{
				if (match[1] == buf[i+1] &&
						match[2] == buf[i+2] &&
						match[3] == buf[i+3])
				{
					if (memcmp(&buf[i], match, match_len) == 0)
					{
						printf("match %d\n", i);
						break;
					}
				}
				i += match_len;
			}
		}
	}
	return 0;
}

int main()
{
	char *a = "asbdoiwfpfqwof_11_nqoekgnwih-13r013jidnancjasnc";
	qstrstr(a, strlen(a), "_11_");
	return 0;
}
