const usersUrl = 'http://localhost:3000/acs/users/';
const adminUrl = 'http://localhost:3000/acs/admin/';

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
  });

  describe('Test Signup page', () => {
    // https://on.cypress.io/assertions
    it('expect - user signup successfully', () => {
      // Arrange:
      const url = 'http://localhost:8000/signup';
      const userName = 'Aviv';
      const userEmail = 'aviv@gmail.com';
      const userAvatar = ':-D';

      // Act:
      cy.visit(url);

      // Assert:
      // Fill user data:
      cy.get('input[id="firstName"]')
        .type(userName)
        .should('have.value', userName);
      cy.get('input[id="email"]')
        .type(userEmail)
        .should('have.value', userEmail);
      cy.get('input[id="avatar"]')
        .type(userAvatar)
        .should('have.value', userAvatar);

      // Press signup:
      cy.get('button[type="submit"]').click();
      cy.url().should('include', '/');
    });
  });
});
