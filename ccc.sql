USE [nfas]
GO
/****** Object:  Table [dbo].[studenttable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[studenttable](
	[studentid] [nvarchar](8) NOT NULL,
	[studentname] [nvarchar](50) NULL,
	[studentsex] [int] NULL,
	[studentclass] [nvarchar](50) NULL,
	[studentacademic] [nvarchar](50) NULL,
	[studentfigure] [nvarchar](64) NULL,
	[intimes] [int] NOT NULL,
	[letimes] [int] NOT NULL,
	[abtimes] [int] NOT NULL,
	[totimes] [int] NOT NULL,
 CONSTRAINT [PK_studenttable] PRIMARY KEY CLUSTERED 
(
	[studentid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[studenttable] ([studentid], [studentname], [studentsex], [studentclass], [studentacademic], [studentfigure], [intimes], [letimes], [abtimes], [totimes]) VALUES (N'cs201401', N'张三', 0, N'cs01', N'1001', N'efdfd028064ac6ce04bf5fe1183a08cc', 2, 0, 0, 2)
INSERT [dbo].[studenttable] ([studentid], [studentname], [studentsex], [studentclass], [studentacademic], [studentfigure], [intimes], [letimes], [abtimes], [totimes]) VALUES (N'cs201402', N'李四', 0, N'cs01', N'1001', N'3a7eacb1e49d57395eb7cff9186eb548', 2, 0, 0, 2)
INSERT [dbo].[studenttable] ([studentid], [studentname], [studentsex], [studentclass], [studentacademic], [studentfigure], [intimes], [letimes], [abtimes], [totimes]) VALUES (N'cs201411', N'赵武', 0, N'cs02', N'1001', N'f88b69691433ad5b6ea233da2dca3566', 1, 1, 0, 2)
INSERT [dbo].[studenttable] ([studentid], [studentname], [studentsex], [studentclass], [studentacademic], [studentfigure], [intimes], [letimes], [abtimes], [totimes]) VALUES (N'cs201412', N'刘三', 0, N'cs02', N'1001', N'70343d5f83375a15d0fbaecf10ea4678', 0, 0, 0, 0)
INSERT [dbo].[studenttable] ([studentid], [studentname], [studentsex], [studentclass], [studentacademic], [studentfigure], [intimes], [letimes], [abtimes], [totimes]) VALUES (N'cs201413', N'黑六', 0, N'cs02', N'1001', N'bef413ad731b07e080d51de24bba2095', 0, 0, 0, 0)
/****** Object:  Table [dbo].[resulttable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[resulttable](
	[resultid] [nvarchar](14) NOT NULL,
	[attendanceid] [nvarchar](8) NULL,
	[theorynumber] [int] NULL,
	[actualnumber] [int] NULL,
	[absencestudent] [nvarchar](1024) NULL,
	[leavestudent] [nvarchar](1024) NULL,
	[submituser] [nvarchar](6) NULL,
 CONSTRAINT [PK_resulttable] PRIMARY KEY CLUSTERED 
(
	[resultid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[resulttable] ([resultid], [attendanceid], [theorynumber], [actualnumber], [absencestudent], [leavestudent], [submituser]) VALUES (N'20180107151147', N'cs001002', 3, 3, NULL, NULL, N'admini')
INSERT [dbo].[resulttable] ([resultid], [attendanceid], [theorynumber], [actualnumber], [absencestudent], [leavestudent], [submituser]) VALUES (N'20180107151257', N'cs001002', 3, 3, NULL, N'cs201411', N'teache')
/****** Object:  Table [dbo].[classtable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[classtable](
	[classid] [nvarchar](4) NULL,
	[classname] [nvarchar](50) NULL,
	[classacademic] [nvarchar](4) NULL,
	[classstudent] [nvarchar](1024) NULL
) ON [PRIMARY]
GO
INSERT [dbo].[classtable] ([classid], [classname], [classacademic], [classstudent]) VALUES (N'cs01', N'计科一班', N'1001', NULL)
INSERT [dbo].[classtable] ([classid], [classname], [classacademic], [classstudent]) VALUES (N'cs02', N'计科二班', N'1001', NULL)
INSERT [dbo].[classtable] ([classid], [classname], [classacademic], [classstudent]) VALUES (N'jh01', N'建环一班', N'1003', NULL)
INSERT [dbo].[classtable] ([classid], [classname], [classacademic], [classstudent]) VALUES (N'ts01', N'测试一班', N'1002', NULL)
/****** Object:  Table [dbo].[attendancetable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[attendancetable](
	[attendanceid] [nvarchar](8) NOT NULL,
	[attendancemachineid] [nvarchar](6) NULL,
	[attendancename] [nvarchar](50) NULL,
	[attendancetime] [nvarchar](50) NULL,
	[attendanceclass] [nvarchar](1024) NULL,
 CONSTRAINT [PK_attendancetable] PRIMARY KEY CLUSTERED 
(
	[attendanceid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[attendancetable] ([attendanceid], [attendancemachineid], [attendancename], [attendancetime], [attendanceclass]) VALUES (N'cs001002', N'01-103', N'计科高数', N'2018/01/07 14:58-2018/01/07 15:43', N'cs01,cs02')
INSERT [dbo].[attendancetable] ([attendanceid], [attendancemachineid], [attendancename], [attendancetime], [attendanceclass]) VALUES (N'cs001003', N'01-211', N'计科英语', N'2018/01/10 21:22-2018/01/10 22:07', N'cs02,cs01')
/****** Object:  Table [dbo].[admintable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[admintable](
	[userid] [nvarchar](6) NOT NULL,
	[userpassword] [nvarchar](50) NULL,
	[useridentify] [int] NULL,
 CONSTRAINT [PK_teachertable] PRIMARY KEY CLUSTERED 
(
	[userid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[admintable] ([userid], [userpassword], [useridentify]) VALUES (N'admini', N'586926', 0)
INSERT [dbo].[admintable] ([userid], [userpassword], [useridentify]) VALUES (N'teache', N'123456', 1)
/****** Object:  Table [dbo].[academictable]    Script Date: 01/09/2018 19:21:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[academictable](
	[academicid] [nvarchar](4) NOT NULL,
	[academicname] [nvarchar](50) NULL,
	[academicclass] [nvarchar](1024) NULL,
 CONSTRAINT [PK_academictable] PRIMARY KEY CLUSTERED 
(
	[academicid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[academictable] ([academicid], [academicname], [academicclass]) VALUES (N'1001', N'信息工程学院', NULL)
INSERT [dbo].[academictable] ([academicid], [academicname], [academicclass]) VALUES (N'1002', N'机电学院', NULL)
INSERT [dbo].[academictable] ([academicid], [academicname], [academicclass]) VALUES (N'1003', N'水利水电学院', NULL)
INSERT [dbo].[academictable] ([academicid], [academicname], [academicclass]) VALUES (N'1004', N'测试学院一', NULL)
/****** Object:  Default [DF_studenttable_abtimes]    Script Date: 01/09/2018 19:21:23 ******/
ALTER TABLE [dbo].[studenttable] ADD  CONSTRAINT [DF_studenttable_abtimes]  DEFAULT ((0)) FOR [intimes]
GO
/****** Object:  Default [DF_studenttable_letimes]    Script Date: 01/09/2018 19:21:23 ******/
ALTER TABLE [dbo].[studenttable] ADD  CONSTRAINT [DF_studenttable_letimes]  DEFAULT ((0)) FOR [letimes]
GO
/****** Object:  Default [DF_studenttable_abtimes_1]    Script Date: 01/09/2018 19:21:23 ******/
ALTER TABLE [dbo].[studenttable] ADD  CONSTRAINT [DF_studenttable_abtimes_1]  DEFAULT ((0)) FOR [abtimes]
GO
/****** Object:  Default [DF_studenttable_totimes]    Script Date: 01/09/2018 19:21:23 ******/
ALTER TABLE [dbo].[studenttable] ADD  CONSTRAINT [DF_studenttable_totimes]  DEFAULT ((0)) FOR [totimes]
GO
