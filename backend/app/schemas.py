from typing import List, Optional
from datetime import datetime
from pydantic import BaseModel


class StatusCreate(BaseModel):
	prototype_id: int
	ph: float
	temperature: float
	humidity: Optional[float] = None
	last_update: str = ''


class Status(StatusCreate):
	id: int

	class Config:
		orm_mode = True


class RecordCreate(BaseModel):
	prototype_id: int
	ph: float
	temperature: float
	humidity: Optional[float] = None
	datetime_creation: str = ''
	

class Record(RecordCreate):
	id: int

	class Config:
		orm_mode = True


class PrototypeCreate(BaseModel):
	owner_name: str


class Prototype(PrototypeCreate):
	id: int
	records: List[Record] = []
	status: List[Status] = {}


	class Config:
		orm_mode = True
