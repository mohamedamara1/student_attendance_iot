import {
  HomeIcon,
  UserCircleIcon,
  TableCellsIcon,
  BellIcon,
  ArrowRightOnRectangleIcon,
  UserPlusIcon,
} from "@heroicons/react/24/solid";
import { Home, Tables } from "@/pages/dashboard";
import { SignIn } from "@/pages/auth";
import LessonPage from "./pages/dashboard/lesson";

const icon = {
  className: "w-5 h-5 text-inherit",
};

export const routes = [
  {
    layout: "dashboard",
    pages: [
      {
        icon: <HomeIcon {...icon} />,
        name: "dashboard",
        path: "/home",
        element: <Home />,
      },
    
      {
        icon: <TableCellsIcon {...icon} />,
        name: "tables",
        path: "/tables",
        element: <Tables />,
        allowedRoles: ["teacher", "admin"]

      },
      {
        icon: <TableCellsIcon {...icon} />,
        name: "lesson",
        path: "/lesson",
        element: <LessonPage />,
        allowedRoles: ["student", "admin"]

      },
    ],
  },
  {
    title: "",
    layout: "auth",
    pages: [
      {
        icon: <ArrowRightOnRectangleIcon {...icon} />,
        name: "sign in",
        path: "/sign-in",
        element: <SignIn />,
        allowedRoles: [ "admin"]

      },
    ],
  },
];

export default routes;
