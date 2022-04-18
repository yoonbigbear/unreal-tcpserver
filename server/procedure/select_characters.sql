USE [Game]
GO
/****** Object:  StoredProcedure [dbo].[select_characters]    Script Date: 2022-04-18 오후 10:49:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER procedure [dbo].[select_characters] @acct_id int
AS
begin
	select top(100)
	[char_id],
	[nickname],
	[class]
	from dbo.character where acct_id = @acct_id;
end
