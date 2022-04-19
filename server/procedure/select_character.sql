
/* 플레이할 캐릭터를 선택*/

if exists (select * from sys.objects where type = 'P' and object_id = object_id('dbo.select_character'))
	drop proc select_character;
go

create procedure select_character(@acct_id int, @char_id bigint)	

as 

select * from dbo.[character] where acct_id = @acct_id and char_id = @char_id;