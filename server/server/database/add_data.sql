USE [Game]
GO
/****** Object:  StoredProcedure [dbo].[add_data]    Script Date: 2022-02-25 오전 12:02:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[add_data] @id bigint
AS
BEGIN
	SET NOCOUNT ON;

	insert game.dbo.table_sample values(@id);

	SET NOCOUNT OFF;
	RETURN 0;

END 
