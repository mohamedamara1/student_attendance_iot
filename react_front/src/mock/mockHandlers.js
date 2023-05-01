// src/mockHandlers.js

import { rest } from 'msw';

export const handlers = [
  rest.post('/api/sign-in', (req, res, ctx) => {
    return res(
      ctx.status(200),
      ctx.json({ userId: 123, userRole: 'admin' })
    );
  }),
];
