use log
go

create procedure add_chat
(
	@chat varchar(64)
)

as
begin
set nocount on;

insert log.dbo.chat_log_temp values(@chat);

set nocount off;
return 0;
end

