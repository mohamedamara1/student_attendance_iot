import { useState } from "react";
import { Link, useNavigate } from "react-router-dom";

import {
  Card,
  CardHeader,
  CardBody,
  CardFooter,
  Input,
  Checkbox,
  Button,
  Typography,
} from "@material-tailwind/react";

export function SignIn() {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [rememberMe, setRememberMe] = useState(false);
  const navigateTo = useNavigate();
  
  const handleEmailChange = (e) => {
    setEmail(e.target.value);
  };

  const handlePasswordChange = (e) => {
    setPassword(e.target.value);
  };

  const handleRememberMeChange = (e) => {
    setRememberMe(e.target.checked);
  };

  const handleSubmit = async (e) => {
    e.preventDefault();

  
    try {


      const response = await fetch('http://192.168.1.8/users/login', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({
            email,
            password
          })
        });

      if (!response.ok) {
        throw new Error('Sign-in failed');
      }
  
      const data = await response.json();
      console.log(data);
      // TODO: Handle response data as needed (e.g. store token in local storage).
      localStorage.setItem('userId', data.userId);
      localStorage.setItem('userRole', data.userRole);

     if (data.userRole == "student"){
      const url = 'http://192.168.1.8/students/findByUserId?userId=' + data.userId;
      const studentResponse = await fetch(url, {
        method: 'GET',
        headers: {
          'Content-Type': 'application/json'
        }
      });
      const studentData = await studentResponse.json()
      console.log(studentData)
      localStorage.setItem('studentId', studentData.id);
      localStorage.setItem('classId', studentData.classId);
      localStorage.setItem('name', studentData.name);
     }
     else if (data.userRole == "teacher"){
      const url = 'http://192.168.1.8/teachers/findByUserId?userId=' + data.userId;
      const teacherResponse = await fetch(url, {
        method: 'GET',
        headers: {
          'Content-Type': 'application/json'
        }
      });
      const teacherData = await teacherResponse.json()
      console.log(teacherData)
      localStorage.setItem('teacherId', teacherData.id);
      localStorage.setItem('name', teacherData.name);
      localStorage.removeItem('studentId');
      localStorage.removeItem('classId');
    }
      navigateTo("/dashboard/home");
    } catch (error) {
      console.error(error);
      // TODO: Display error message to user (e.g. using a toast message).
    }
  };
  
  return (
    <>
      <img
        src="https://images.unsplash.com/photo-1497294815431-9365093b7331?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=1950&q=80"
        className="absolute inset-0 z-0 h-full w-full object-cover"
      />
      <div className="absolute inset-0 z-0 h-full w-full bg-black/50" />
      <div className="container mx-auto p-4">
        <Card className="absolute top-2/4 left-2/4 w-full max-w-[24rem] -translate-y-2/4 -translate-x-2/4">
          <CardHeader
            variant="gradient"
            color="blue"
            className="mb-4 grid h-28 place-items-center"
          >
            <Typography variant="h3" color="white">
              Sign In
            </Typography>
          </CardHeader>
          <CardBody className="flex flex-col gap-4">
            <Input type="email" label="Email" size="lg" onChange={handleEmailChange}/>
            <Input type="password" label="Password" size="lg" onChange={handlePasswordChange} />
            <div className="-ml-2.5">
              <Checkbox label="Remember Me" />
            </div>
          </CardBody>
          <CardFooter className="pt-0">
            <Button variant="gradient" fullWidth onClick={handleSubmit}>
              Sign In
            </Button>
            <Typography variant="small" className="mt-6 flex justify-center">
              Don't have an account?
              <Link to="/auth/sign-up">
                <Typography
                  as="span"
                  variant="small"
                  color="blue"
                  className="ml-1 font-bold"
                >
                  Sign up
                </Typography>
              </Link>
            </Typography>
          </CardFooter>
        </Card>
      </div>
    </>
  );
}

export default SignIn;
