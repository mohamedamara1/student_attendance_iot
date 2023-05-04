import {

    Chip
  } from "@material-tailwind/react";


const attendancesTableData = [
    {
      id: 1,
    // lesson_id: 1, // add lesson id
      student_name: "John Doe",
      course_name: "Mathematics",
      class_name: "A",
 //     date: "2023-05-01",
   //   start_time: "08:00",
      present: true,
    }
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
