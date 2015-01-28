#ifndef _IMSERRCODE_H_
#define _IMSERRCODE_H_

#define IMSGETERRCODE(x) ((x) & 0x0000FFFF)

#define IMSERR_OK			0x0L

#define IMSERR_INVALIDVER	

#define IMSERR_NOTINIT		

/* Completion Cause for Synthesizer resource
+------------+-----------------------+------------------------------+
| Cause-Code | Cause-Name            | Description                  |
+------------+-----------------------+------------------------------+
| 000        | normal                | SPEAK completed normally.    |
| 001        | barge-in              | SPEAK request was terminated |
|            |                       | because of barge-in.         |
| 002        | parse-failure         | SPEAK request terminated     |
|            |                       | because of a failure to      |
|            |                       | parse the speech markup      |
|            |                       | text.                        |
| 003        | uri-failure           | SPEAK request terminated     |
|            |                       | because access to one of the |
|            |                       | URIs failed.                 |
| 004        | error                 | SPEAK request terminated     |
|            |                       | prematurely due to           |
|            |                       | synthesizer error.           |
| 005        | language-unsupported  | Language not supported.      |
| 006        | lexicon-load-failure  | Lexicon loading failed.      |
+------------+-----------------------+------------------------------+
*/

#define IMSERR_SYN_NORMAL						0
#define IMSERR_SYN_BARGEIN						1
#define IMSERR_SYN_PARSE						2
#define IMSERR_SYN_URI							3
#define IMSERR_SYN_ERROR						4
#define IMSERR_SYN_UNSUPP_LANG					5
#define IMSERR_SYN_LOAD_LEX						6

#endif
