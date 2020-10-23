const usersUrl = 'http://localhost:3000/acs/users/';
const adminUrl = 'http://localhost:3000/acs/admin/';

const playerUser = {
  email: 'avivPlayer@gmail.com',
  role: 'PLAYER',
  username: 'aviv-test-player',
  avatar: ':-)',
};

const adminUser = {
  email: 'avivAdmin@gmail.com',
  role: 'ADMIN',
  username: 'aviv-test-admin',
  avatar: ':-)',
};

context('Assertions', () => {
  beforeEach(() => {
    // Create an admin user:
    cy.request('POST', usersUrl, adminUser);

    // Delete all the users from the server:
    const deleteAllUsersUrl = `${adminUrl}users/${adminUser.email}`;
    cy.request('DELETE', deleteAllUsersUrl);

    // Create an player user:
    cy.request('POST', usersUrl, playerUser);
  });

  describe('Test Signin page', () => {
    // https://on.cypress.io/assertions
    it('expect - user sign successfully', () => {
      // Arrange:
      const url = 'http://localhost:8000/signin';
      const userEmail = 'avivPlayer@gmail.com';

      // Act:
      cy.visit(url);

      // Assert:
      // Fill user data:
      cy.get('input[id="email"]')
        .type(userEmail)
        .should('have.value', userEmail);

      // Press signup:
      cy.get('button[type="submit"]').click();
      cy.url().should('include', '/');
    });
  });
});
