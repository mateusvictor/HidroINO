from datetime import datetime


def str_datetime_now():
	now = datetime.now()
	return now.strftime('%d-%m-%Y %H:%M:%S')
