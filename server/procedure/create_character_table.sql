


use Game
drop table dbo.character

create table character (
char_id bigint not null primary key,
acct_id int not null,
nickname nvarchar(8) not null,
class smallint not null)

create index acct_id_key on dbo.character (acct_id ASC)