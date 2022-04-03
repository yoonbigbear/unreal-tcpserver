
DROP PROC IF EXISTS [dbo].[create_table_sample]
GO

CREATE PROCEDURE [dbo].[create_table_sample] @id bigint
AS
BEGIN

	IF NOT EXISTS (SELECT * FROM sysobjects WHERE name='table_sample')
	CREATE TABLE [dbo].[table_sample](
	[id] [bigint] NOT NULL);

	insert game.dbo.table_sample values(@id);

	Select * from game.dbo.table_sample

	RETURN 0;

END 
GO


