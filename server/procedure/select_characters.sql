USE [Game]
GO
/****** Object:  StoredProcedure [dbo].[select_characters]    Script Date: 2022-05-13 오전 1:20:25 ******/
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
