use Game
go

create procedure select_character_nickname(@nickname varchar(64))

as

if exists (
select * from dbo.[character] where nickname = @nickname
)
begin
return -1;
end

else
begin
return 0;
end