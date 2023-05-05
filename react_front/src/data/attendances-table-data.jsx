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

  const handleToggleAttendance = (record, attendances, setAttendances) => {
    const newPresent = !record.present;
    const url = `http://192.168.4.1/attendances?id=${record.id}`;
    const data = { present: newPresent };
    console.log(attendances)

    fetch(url, {
      method: 'PUT',
      body: JSON.stringify(data),
    }).then(() => {
      // Update the state to reflect the change
      const updatedData = attendances.map((item) => {
        if (item.id === record.id) {
          return { ...item, present: newPresent };
        }
        return item;
      });
      setAttendances(updatedData);
    }).catch((error) => {
      console.error('Error updating attendance record:', error);
    });
  }

  const attendancesTableColumns = {
    title: "Attendances Table",
    headers: Object.keys(attendancesTableData[0]),
    columns: Object.keys(attendancesTableData[0]).map((key) => ({
      accessor: key,
      render: (value, props) => {
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
                className="py-0.5 px-2 text-[11px] font-medium cursor-pointer transform transition duration-200 hover:scale-110"
                onClick={() => handleToggleAttendance(props.row, props.attendances, props.setAttendances )}
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
