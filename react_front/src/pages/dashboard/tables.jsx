import { useState, useEffect } from "react";

import {
  Card,
  CardHeader,
  CardBody,
  Typography,
  Avatar,
  Chip,
  Tooltip,
  Progress,
} from "@material-tailwind/react";

import { lessonsTableData, lessonsTableColumns, attendancesTableColumns, attendancesTableData } from "@/data";
import Table from "@/widgets/table";

export function Tables() {
  const [attendances, setAttendances] = useState(attendancesTableData);
  const [lessons, setLessons] = useState([]);
  const teacherId= localStorage.getItem("teacherId");

  async function fetchLessonsTableData() {
    try {
      const response = await fetch(`http://192.168.1.8/lessons/findByTeacherId?teacherId=${teacherId}`);
      const data = await response.json();
      setLessons(data);
    } catch (error) {
      console.error(error);
    }
  }

  useEffect(() => {
    fetchLessonsTableData();
  }, []);

  async function handleLessonClick(lessonId) {
    console.log(lessonId);
    try {
      const response = await fetch(`http://192.168.1.8/attendances/findByLessonId?lessonId=${lessonId}`);
      const filteredData = await response.json();
      setAttendances(filteredData);
    } catch (error) {
      console.error(error);
    }
  }

  return (
    <div className="mt-12 mb-8 flex flex-col gap-12">
     
      <Table data={attendances} columns={attendancesTableColumns} />
      <Table data={lessons} columns={lessonsTableColumns} handleRowClick={handleLessonClick} />          
      
    </div>
  );
}

export default Tables;
