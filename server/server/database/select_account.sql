USE [account]
GO
/****** Object:  StoredProcedure [dbo].[select_account]    Script Date: 2022-04-15 오전 12:30:51 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER procedure [dbo].[select_account]
@id varchar(16),
@password varchar(16)

AS

SET NOCOUNT ON;

select * from account.dbo.account where [id] = @id and [password] = @password

SET NOCOUNT OFF;