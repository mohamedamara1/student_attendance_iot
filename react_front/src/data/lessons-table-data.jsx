const lessonsTableData = [
  {
    "id": 1,
    "Course": "math",
    "Class": "1ing",
    "Classroom": 0,
    "Date": 20,
    "Starttime": 9,
    "Endtime": 11
}
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
  