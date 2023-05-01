import { useState } from "react";

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

import { EllipsisVerticalIcon } from "@heroicons/react/24/outline";
import { authorsTableData, projectsTableData, lessonsTableData, lessonsTableColumns, attendancesTableColumns, attendancesTableData } from "@/data";
import Table from "@/widgets/table";

export function Tables() {
  const [attendances, setAttendances] = useState(attendancesTableData);
   async function handleLessonClick(lessonId) {
    console.log(lessonId);
    try {
      const filteredData = attendancesTableData.filter((attendance) => attendance.lesson_id === lessonId);
      setAttendances(filteredData);
    } catch (error) {
      console.error(error);
    }
  }

  return (
    <div className="mt-12 mb-8 flex flex-col gap-12">
     
      <Table data={attendances} columns={attendancesTableColumns} />
      <Table data={lessonsTableData} columns={lessonsTableColumns} handleRowClick={handleLessonClick} />          
      
    </div>
  );
}

export default Tables;
