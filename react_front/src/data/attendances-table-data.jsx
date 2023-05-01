import {

    Chip
  } from "@material-tailwind/react";


const attendancesTableData = [
    {
      id: 1,
      lesson_id: 1, // add lesson id
      student_name: "John Doe",
      course_name: "Mathematics",
      class_name: "A",
      date: "2023-05-01",
      start_time: "08:00",
      present: true,
    },
    {
      id: 2,
      lesson_id: 1, // add lesson id
      student_name: "Jane Smith",
      course_name: "Science",
      class_name: "B",
      date: "2023-05-02",
      start_time: "10:00",
      present: false,
    },
    {
      id: 3,
      lesson_id: 2, // add lesson id
      student_name: "Bob Johnson",
      course_name: "English",
      class_name: "C",
      date: "2023-05-03",
      start_time: "13:00",
      present: true,
    },
    {
      id: 4,
      lesson_id: 2, // add lesson id
      student_name: "Alice Lee",
      course_name: "History",
      class_name: "D",
      date: "2023-05-04",
      start_time: "15:00",
      present: false,
    },
    {
      id: 5,
      lesson_id: 3, // add lesson id
      student_name: "Tom Brown",
      course_name: "Geography",
      class_name: "E",
      date: "2023-05-05",
      start_time: "17:00",
      present: true,
    },
    {
      id: 6,
      lesson_id: 3, // add lesson id
      student_name: "Sarah Jones",
      course_name: "Biology",
      class_name: "F",
      date: "2023-05-06",
      start_time: "19:00",
      present: false,
    },
  ];
  

  const attendancesTableColumns = {
    title: "Attendances Table",
    headers: Object.keys(attendancesTableData[0]),
    columns: Object.keys(attendancesTableData[0]).map((key) => ({
      accessor: key,
      render: (value) => {
          const className = `py-3`;
        if (key === 'present') {
          const present = value;
          const chipValue = present ? 'Present' : 'Absent';
          const chipColor = present ? 'green' : 'blue-gray';
          return (
            <td className={className}>
              <Chip
                variant="gradient"
                color={chipColor}
                value={chipValue}
                className="py-0.5 px-2 text-[11px] font-medium"
              />
            </td>
          );
        } else {
          return <span>{value}</span>;
        }
      },
    })),
  };
  

  export { attendancesTableColumns, attendancesTableData };
