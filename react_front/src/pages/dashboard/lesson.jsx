import React, { useState, useEffect  } from 'react';
import { Card, CardBody, Button, Alert, CardHeader} from "@material-tailwind/react";
import { response } from 'msw';


const LessonPage = (props) => {
    const [attendance, setAttendance] = useState(false);
    const [lesson, setLesson] = useState(null);


    const studentId = localStorage.getItem('studentId');

  useEffect(() => {
    // Fetch the latest lesson
    fetch(`http://192.168.1.8/lesson/newestLesson?student_id=${studentId}`)
      .then(response => response.json())
      .then(response => response.data)
      .then(data => {
        setLesson(data);
        console.log(data);
      })
      .catch(error => {
        console.error('Error fetching latest lesson:', error);
      });
  }, [studentId]);

  const handleAttendance = () => {
    const studentId = localStorage.getItem('studentId');
    const lessonId = lesson.id;
    const present = true;
  
    fetch('http://192.168.1.8/attendances/markPresence', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        studentId: parseInt(studentId),
        lessonId: lessonId
      })
    })
    .then(response => response.json())
    .then(data => {
      console.log('Attendance object created:', data);
      setAttendance(true);
    })
    .catch(error => {
      console.error('Error creating attendance object:', error);
    });
  };
  

    if (!lesson) {
      return <div>Loading...</div>;
    }
    const { date, start_time, end_time, course_name: courseName, classroom_name, teacher_name: teacherName } = lesson;

    return (
            <div className="flex justify-center pt-8">
              <Card>
                <CardHeader color="indigo" contentPosition="left">
                  <h2 className="text-white text-2xl">Course Name: {courseName}</h2>
                </CardHeader>
                <CardBody>
                  <div className="flex flex-col items-center">
                  <h6 className="text-lg">Teacher: {teacherName}</h6>
            <h1 className="text-2xl font-medium mt-2">Date: {date}</h1>
            <h2 className="text-lg font-medium mt-2">Time: {start_time} - {end_time}</h2>

                    {!attendance && (
                      <Button
                        color="indigo"
                        buttonType="filled"
                        size="lg"
                        block={true}
                        className="mt-8"
                        onClick={handleAttendance}
                      >
                        Mark Attendance
                      </Button>
                    )}
          
                    {attendance && (
                      <Alert type="success" onClose={() => setAttendance(false)} className="mt-8">
                        Attendance Marked!
                      </Alert>
                    )}
                  </div>
                </CardBody>
                <CardBody></CardBody>
              </Card>
            </div>
          );   
};

export default LessonPage;
