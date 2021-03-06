USE [Game]
GO
/****** Object:  StoredProcedure [dbo].[create_character]    Script Date: 2022-04-25 오전 12:02:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER procedure [dbo].[create_character](@char_id bigint, @acct_id int, @nickname nvarchar(16), @class smallint)

as

if exists (
select * from dbo.[character] where nickname = @nickname
)
begin
return -1;
end

else
begin
insert into Game.dbo.character values(@char_id, @acct_id, @nickname, @class, 0,0,0)
return 0;
end