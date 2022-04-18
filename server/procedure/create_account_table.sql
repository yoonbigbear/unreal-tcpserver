

drop table dbo.account

create table account (
acct_id int not null Identity(1,1),
id varchar(16) not null,
password varchar(16) not null)