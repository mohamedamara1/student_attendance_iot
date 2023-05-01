const lessonsTableData = [
    {
      id: 1,
      course_id: 101,
      class_id: "A",
      teacher_id: 1001,
      classroom_id: 201,
      date: "2023-05-01",
      start_time: "08:00",
      end_time: "09:30",
    },
    {
      id: 2,
      course_id: 102,
      class_id: "B",
      teacher_id: 1002,
      classroom_id: 202,
      date: "2023-05-02",
      start_time: "10:00",
      end_time: "11:30",
    },
    {
      id: 3,
      course_id: 103,
      class_id: "C",
      teacher_id: 1003,
      classroom_id: 203,
      date: "2023-05-03",
      start_time: "13:00",
      end_time: "14:30",
    },
  ];

const lessonsTableColumns = {
title: "Lessons Table",
headers: Object.keys(lessonsTableData[0]),
columns: Object.keys(lessonsTableData[0]).map((key) => ({
  accessor: key,
  render: (value) => <span>{value}</span>,
})),
};
  

export { lessonsTableData, lessonsTableColumns };
  